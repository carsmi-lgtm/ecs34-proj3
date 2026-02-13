#include "XMLReader.h"
#include "StreetMap.h"

// OSM implementation code - incomplete

struct COpenStreetMap::SImplementation
{

    struct SNode : public CStreetMap::SNode
    {
        ~SNode()
        {
        }
        TNodeID ID() const noexcept override
        {
        }
        TLocation Location() const noexcept override
        {
        }
        std::size_t AttributeCount() const noexcept override
        {
        }
        std::string GetAttributeKey(std::size_t index) const noexcept override
        {
        }
        bool HasAttribute(const std::string &key) const noexcept override
        {
        }
        std::string GetAttribute(const std::string &key) const noexcept override
        {
        }
    };

    struct SWay : public CStreetMap::SWay
    {
        ~SWay() {

        };
        TWayID ID() const noexcept override
        {
        }
        std::size_t NodeCount() const noexcept override
        {
        }
        TNodeID GetNodeID(std::size_t index) const noexcept override
        {
        }
        std::size_t AttributeCount() const noexcept override
        {
        }
        std::string GetAttributeKey(std::size_t index) const noexcept override
        {
        }
        bool HasAttribute(const std::string &key) const noexcept override
        {
        }
        std::string GetAttribute(const std::string &key) const noexcept override
        {
        }
    };

    SImplementation(std::shared_ptr<CXMLReader>)

        std::size_t
        COpenStreetMap::NodeCount() const noexcept;
    std::size_t COpenStreetMap::WayCount() const noexcept;
    std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept;
    std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept;
    std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept;
    std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept;
};