#include <gtest/gtest.h>
#include "CSVBusSystem.h"
#include "StringDataSource.h"

TEST(CSVBusSystem, SimpleFiles)
{
    auto StopDataSource = std::make_shared<CStringDataSource>("stop_id,node_id\n"
                                                              "1,123\n"
                                                              "2,124");
    auto StopReader = std::make_shared<CDSVReader>(StopDataSource);
    auto RouteDataSource = std::make_shared<CStringDataSource>("route,stop_id\n"
                                                               "A,1\n"
                                                               "A,2");
    auto RouteReader = std::make_shared<CDSVReader>(RouteDataSource);

    CCSVBusSystem BusSystem(StopReader, RouteReader);

    EXPECT_EQ(BusSystem.StopCount(), 2);

    auto StopObjInd0 = BusSystem.StopByIndex(0);
    EXPECT_NE(StopObjInd0, nullptr);

    auto StopObjInd1 = BusSystem.StopByIndex(1);
    EXPECT_NE(StopObjInd1, nullptr);

    auto StopObjID1 = BusSystem.StopByID(1);
    ASSERT_NE(StopObjID1, nullptr);
    EXPECT_EQ(StopObjID1->ID(), 1);
    EXPECT_EQ(StopObjID1->NodeID(), 123);

    auto StopObjID2 = BusSystem.StopByID(2);
    ASSERT_NE(StopObjID2, nullptr);
    EXPECT_EQ(StopObjID2->ID(), 2);
    EXPECT_EQ(StopObjID2->NodeID(), 124);

    EXPECT_EQ(BusSystem.RouteCount(), 1);

    auto RouteObjInd0 = BusSystem.RouteByIndex(0);
    ASSERT_NE(RouteObjInd0, nullptr);
    EXPECT_EQ(RouteObjInd0->Name(), "A");
    EXPECT_EQ(RouteObjInd0->GetStopID(0), 1);
    EXPECT_EQ(RouteObjInd0->StopCount(), 2);
    EXPECT_EQ(RouteObjInd0->GetStopID(1), 2);

    auto RouteObjNameA = BusSystem.RouteByName("A");
    ASSERT_NE(RouteObjNameA, nullptr);
    EXPECT_EQ(RouteObjNameA->GetStopID(0), 1);
    EXPECT_EQ(RouteObjNameA->GetStopID(1), 2);

    EXPECT_EQ(BusSystem.RouteByIndex(12), nullptr);
    EXPECT_EQ(BusSystem.RouteByName("B"), nullptr);
    EXPECT_EQ(BusSystem.StopByID(500), nullptr);
}

TEST(CSVBusSystem, InvalidHeaders)
{
    auto BadStopData = std::make_shared<CStringDataSource>("stop, node\n1,123\n2,124"); // invalid headers
    auto BadStopReader = std::make_shared<CDSVReader>(BadStopData);
    auto BadRouteData = std::make_shared<CStringDataSource>("route_id,stop\nA,3\nA,4\nB,5"); // invalid headers
    auto BadRouteReader = std::make_shared<CDSVReader>(BadRouteData);
    CCSVBusSystem BadBusSystem(BadStopReader, BadRouteReader);

    // should all return 0 or nullptr because reader returns false after invalid headers are read
    EXPECT_EQ(BadBusSystem.StopCount(), 0);
    EXPECT_EQ(BadBusSystem.RouteCount(), 0);
    EXPECT_EQ(BadBusSystem.StopByIndex(0), nullptr);
    EXPECT_EQ(BadBusSystem.StopByID(123), nullptr);
    EXPECT_EQ(BadBusSystem.RouteByIndex(0), nullptr);
    EXPECT_EQ(BadBusSystem.RouteByName("A"), nullptr);
}