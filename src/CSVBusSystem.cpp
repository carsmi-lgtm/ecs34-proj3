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
            return "";
        }

        std::size_t StopCount() const noexcept
        {
            return 0;
        }

        TStopID GetStopID(std::size_t index) const noexcept
        {
            return 0;
        }
    };

    const std::string STOP_ID_HEADER = "stop_id";
    const std::string NODE_ID_HEADER = "node_id";
    const std::string ROUTE_ID_HEADER = "route";

    std::vector<std::shared_ptr<SStop>> DStopsByIndex;
    std::unordered_map<TStopID, std::shared_ptr<SStop>> DStopsByID;

    // csv reading stops method
    bool ReadStops(std::shared_ptr<CDSVReader> stopsrc)
    {
        std::vector<std::string> TempRow;
        stopsrc->ReadRow(TempRow); // read header row

        if (stopsrc->ReadRow(TempRow))
        { // read second row, start parsing
            size_t StopColumn = -1;
            size_t NodeColumn = -1;

            // identify stop column and node column
            for (size_t Index = 0; Index < TempRow.size(); Index++)
            {

                if (TempRow[Index] == STOP_ID_HEADER)
                {
                    StopColumn = Index;
                }
                else if (TempRow[Index] == NODE_ID_HEADER)
                {
                    NodeColumn = Index;
                }
            }
            if (StopColumn == -1 || NodeColumn == -1)
                return false;

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

    // csv read routes method
    bool ReadRoutes(std::shared_ptr<CDSVReader> routesrc)
    {
        std::vector<std::string> TempRow;
        routesrc->ReadRow(TempRow); // read first row (header row)

        if (routesrc->ReadRow(TempRow))
        { // read second row, start parsing
            size_t RouteColumn = -1;
            size_t StopColumn = -1;

            // identify route and stop column
            for (size_t Index = 0; Index < TempRow.size(); Index++)
            {

                if (TempRow[Index] == ROUTE_ID_HEADER)
                {
                    RouteColumn = Index;
                }
                else if (TempRow[Index] == STOP_ID_HEADER)
                {
                    StopColumn = Index;
                }
            }
            if (RouteColumn == -1 || StopColumn == -1)
                return false;

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

                // NEED TO ADD moving routes into implementation storage
            }

            return true;
        }
        return false;
    }

    // constructor
    SImplementation(std::shared_ptr<CDSVReader> stopsrc, std::shared_ptr<CDSVReader> routesrc)
    {
        if (ReadStops(stopsrc))
        {
            // add route reading here
        }
    }

    // ACCESSORS

    std::size_t StopCount() const noexcept
    {
        return DStopsByIndex.size();
    }
    std::size_t RouteCount() const noexcept
    {
        return 0; // edit after implementing routes
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
        return nullptr; // edit after implementing routes
    }
    std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept
    {
        return nullptr; // edit after implementing routes
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
