#include "CSVBusSystem.h"
#include <unordered_map>

// implementation struct
struct CCSVBusSystem::SImplementation
{

    // derived stop struct
    struct SStop : public CBusSystem::SStop
    {
        TStopID DID;                 // stop id
        CStreetMap::TNodeID DNodeID; // corresponding node id

        // constructor
        SStop(TStopID id, CStreetMap::TNodeID node_id) : DID(id), DNodeID(node_id) {}

        // destructor
        ~SStop() override = default;

        // stop id accessor
        TStopID ID() const noexcept override
        {
            return DID;
        }

        // node id accessor
        CStreetMap::TNodeID NodeID() const noexcept override
        {
            return DNodeID;
        }
    };

    // derived route struct
    struct SRoute : public CBusSystem::SRoute
    {
    public:
        // public data members
        std::string DName;
        std::vector<TStopID> DStopIDs;

        // destructor
        ~SRoute() override = default;

        std::string Name() const noexcept
        {
            return DName;
        }

        std::size_t StopCount() const noexcept
        {
            return DStopIDs.size();
        }

        TStopID GetStopID(std::size_t index) const noexcept
        {
            if (index >= DStopIDs.size())
                return 0;
            return DStopIDs[index];
        }
    };

    // variables for header strings
    const std::string STOP_ID_HEADER = "stop_id";
    const std::string NODE_ID_HEADER = "node_id";
    const std::string ROUTE_ID_HEADER = "route";

    // stops storage
    std::vector<std::shared_ptr<SStop>> DStopsByIndex;
    std::unordered_map<TStopID, std::shared_ptr<SStop>> DStopsByID;

    // csv reading stops method
    bool ReadStops(std::shared_ptr<CDSVReader> stopsrc)
    {
        std::vector<std::string> TempRow;
        if (!stopsrc->ReadRow(TempRow))
            return false; // read header row

        else
        { // set StopCol and NodeCol to -1, indicated not yet read
            int StopCol = -1;
            int NodeCol = -1;

            // identify stop column and node column, return false if not read
            for (size_t Index = 0; Index < TempRow.size(); Index++)
            {

                if (TempRow[Index] == STOP_ID_HEADER)
                {
                    StopCol = Index;
                }
                else if (TempRow[Index] == NODE_ID_HEADER)
                {
                    NodeCol = Index;
                }
            }
            if (StopCol == -1 || NodeCol == -1)
                return false;

            // cast StopColumn and NodeColumn to size_t
            size_t StopColumn = static_cast<size_t>(StopCol);
            size_t NodeColumn = static_cast<size_t>(NodeCol);

            // parse remaining rows into stops
            while (stopsrc->ReadRow(TempRow))
            {
                TStopID StopID = std::stoull(TempRow[StopColumn]);
                CStreetMap::TNodeID NodeID = std::stoull(TempRow[NodeColumn]);
                auto NewStop = std::make_shared<SStop>(StopID, NodeID);
                DStopsByIndex.push_back(NewStop);
                DStopsByID[StopID] = NewStop;
            }
            return true;
        }

        return false;
    }

    // routes storage
    std::vector<std::shared_ptr<SRoute>> DRoutesByIndex;
    std::unordered_map<std::string, std::shared_ptr<SRoute>> DRoutesByName;

    // csv read routes method
    bool ReadRoutes(std::shared_ptr<CDSVReader> routesrc)
    {
        std::vector<std::string> TempRow;
        if (!routesrc->ReadRow(TempRow))
            return false; // read first row (header row)

        else
        { // initiate to -1 indicating it hasn't been read
            int RouteCol = -1;
            int StopCol = -1;

            // identify route and stop column, return false if not read
            for (size_t Index = 0; Index < TempRow.size(); Index++)
            {

                if (TempRow[Index] == ROUTE_ID_HEADER)
                {
                    RouteCol = Index;
                }
                else if (TempRow[Index] == STOP_ID_HEADER)
                {
                    StopCol = Index;
                }
            }
            if (RouteCol == -1 || StopCol == -1)
                return false;

            // cast RouteColumn and StopColumn to size_t
            size_t RouteColumn = static_cast<size_t>(RouteCol);
            size_t StopColumn = static_cast<size_t>(StopCol);

            std::unordered_map<std::string, std::shared_ptr<SRoute>> RoutesMap;

            // parse remaining rows into routes
            while (routesrc->ReadRow(TempRow))
            {
                std::string RouteName = TempRow[RouteColumn];
                TStopID StopID = std::stoull(TempRow[StopColumn]);

                // check if route exists in map
                auto It = RoutesMap.find(RouteName);
                if (It == RoutesMap.end())
                {
                    auto NewRoute = std::make_shared<SRoute>();
                    NewRoute->DName = RouteName;
                    NewRoute->DStopIDs.push_back(StopID);
                    RoutesMap[RouteName] = NewRoute;
                }
                else
                {
                    It->second->DStopIDs.push_back(StopID);
                }
            }
            // move routes into storage
            for (auto &Route : RoutesMap)
            {
                DRoutesByIndex.push_back(Route.second);
                DRoutesByName[Route.first] = Route.second;
            }

            return true;
        }
    }

    // constructor
    SImplementation(std::shared_ptr<CDSVReader> stopsrc, std::shared_ptr<CDSVReader> routesrc)
    {
        if (ReadStops(stopsrc))
        {
            ReadRoutes(routesrc);
        }
    }

    // ACCESSORS

    std::size_t StopCount() const noexcept
    {
        return DStopsByIndex.size();
    }
    std::size_t RouteCount() const noexcept
    {
        return DRoutesByIndex.size(); // edit after implementing routes
    }
    std::shared_ptr<SStop> StopByIndex(std::size_t index) const noexcept
    {
        if (index >= DStopsByIndex.size())
            return nullptr;
        return DStopsByIndex[index];
    }
    std::shared_ptr<SStop> StopByID(TStopID id) const noexcept
    {
        auto It = DStopsByID.find(id);
        if (It == DStopsByID.end())
            return nullptr;
        return It->second;
    }
    std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const noexcept
    {
        if (index >= DRoutesByIndex.size())
            return nullptr;
        return DRoutesByIndex[index];
    }
    std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept
    {
        auto It = DRoutesByName.find(name);
        if (It == DRoutesByName.end())
        {
            return nullptr;
        }
        return It->second;
    }
};

// public CSVBusSystem constructor
CCSVBusSystem::CCSVBusSystem(std::shared_ptr<CDSVReader> stopsrc, std::shared_ptr<CDSVReader> routesrc)
{
    DImplementation = std::make_unique<SImplementation>(stopsrc, routesrc);
}

// public CSVBusSystem destructor
CCSVBusSystem::~CCSVBusSystem()
{
}

// stop/route accessors
std::size_t CCSVBusSystem::StopCount() const noexcept
{
    return DImplementation->StopCount();
}

std::size_t CCSVBusSystem::RouteCount() const noexcept
{
    return DImplementation->RouteCount();
}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByIndex(std::size_t index) const noexcept
{
    return DImplementation->StopByIndex(index);
}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByID(TStopID id) const noexcept
{
    return DImplementation->StopByID(id);
}

std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByIndex(std::size_t index) const noexcept
{
    return DImplementation->RouteByIndex(index);
}

std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByName(const std::string &name) const noexcept
{
    return DImplementation->RouteByName(name);
}
