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

    // reader returns false after invalid headers are read -> no data read, no stops or routes, all pointers null
    EXPECT_EQ(BadBusSystem.StopCount(), 0);
    EXPECT_EQ(BadBusSystem.RouteCount(), 0);
    EXPECT_EQ(BadBusSystem.StopByIndex(0), nullptr);
    EXPECT_EQ(BadBusSystem.StopByID(123), nullptr);
    EXPECT_EQ(BadBusSystem.RouteByIndex(0), nullptr);
    EXPECT_EQ(BadBusSystem.RouteByName("A"), nullptr);
}

TEST(CSVBusSystem, InvalidParams)
{
    auto BadStopData = std::make_shared<CStringDataSource>("stop_id,node_id\n1,123\n2,124"); // valid headers
    auto BadStopReader = std::make_shared<CDSVReader>(BadStopData);
    auto BadRouteData = std::make_shared<CStringDataSource>("route,stop_id\nA,1\nA,2\nB,2\nC,2"); // valid headers
    auto BadRouteReader = std::make_shared<CDSVReader>(BadRouteData);
    CCSVBusSystem BadBusSystem(BadStopReader, BadRouteReader);

    // test valid parameters first
    auto Stop0 = BadBusSystem.StopByIndex(0);
    ASSERT_NE(Stop0, nullptr);
    EXPECT_EQ(Stop0->ID(), 1);
    EXPECT_EQ(Stop0->NodeID(), 123);

    auto RouteA = BadBusSystem.RouteByName("A");
    ASSERT_NE(RouteA, nullptr);
    EXPECT_EQ(RouteA->Name(), "A");
    EXPECT_EQ(RouteA->StopCount(), 2);
    EXPECT_EQ(RouteA->GetStopID(1), 2);

    EXPECT_EQ(BadBusSystem.StopCount(), 2);
    EXPECT_EQ(BadBusSystem.RouteCount(), 3);

    // test invalid parameters: indices, ID's, and names that do not exist
    EXPECT_EQ(BadBusSystem.StopByIndex(5), nullptr);
    EXPECT_EQ(BadBusSystem.StopByID(300), nullptr);
    EXPECT_EQ(BadBusSystem.RouteByIndex(10), nullptr);
    EXPECT_EQ(BadBusSystem.RouteByName("D"), nullptr);
}

TEST(CSVBusSystem, EmptyFiles)
{
    auto EmptyStopData = std::make_shared<CStringDataSource>("stop_id,node_id\n"); // no data, only headers
    auto EmptyStopReader = std::make_shared<CDSVReader>(EmptyStopData);
    auto EmptyRouteData = std::make_shared<CStringDataSource>("route,stop_id\n"); // no data, only headers
    auto EmptyRouteReader = std::make_shared<CDSVReader>(EmptyRouteData);
    CCSVBusSystem EmptyBusSystem(EmptyStopReader, EmptyRouteReader);

    // no routes, no stops, all other pointers are null
    EXPECT_EQ(EmptyBusSystem.StopCount(), 0);
    EXPECT_EQ(EmptyBusSystem.StopByIndex(0), nullptr);
    EXPECT_EQ(EmptyBusSystem.StopByID(0), nullptr);
    EXPECT_EQ(EmptyBusSystem.RouteCount(), 0);
    EXPECT_EQ(EmptyBusSystem.RouteByIndex(0), nullptr);
    EXPECT_EQ(EmptyBusSystem.RouteByName("A"), nullptr);
}

TEST(CSVBusSystem, EmptyValues)
{
    auto StopData = std::make_shared<CStringDataSource>("stop_id,node_id\n1,\n2,"); // missing node ID
    auto StopReader = std::make_shared<CDSVReader>(StopData);
    auto RouteData = std::make_shared<CStringDataSource>("route,stop_id\nA,1\nB,2");
    auto RouteReader = std::make_shared<CDSVReader>(RouteData);

    // should not construct bus system without correct data
    EXPECT_THROW(CCSVBusSystem(StopReader, RouteReader), std::invalid_argument);
}

TEST(CSVBusSystem, DuplicateStopID)
{
    auto StopData = std::make_shared<CStringDataSource>("stop_id,node_id\n1,111\n1,222\n1,333"); // three node id's for same stop
    auto StopReader = std::make_shared<CDSVReader>(StopData);
    auto RouteData = std::make_shared<CStringDataSource>("route,stop_id\nA,1\nB,1");
    auto RouteReader = std::make_shared<CDSVReader>(RouteData);
    CCSVBusSystem BusSystem(StopReader, RouteReader);

    // only one stop should exist

    EXPECT_EQ(BusSystem.StopCount(), 1);

    auto StopObj1 = BusSystem.StopByIndex(0);
    ASSERT_NE(StopObj1, nullptr);
    EXPECT_EQ(StopObj1->NodeID(), 111);
    EXPECT_NE(StopObj1->NodeID(), 333);
    EXPECT_EQ(StopObj1->ID(), 1);

    EXPECT_EQ(BusSystem.StopByIndex(1), nullptr);

    auto StopObj2 = BusSystem.StopByID(1);
    ASSERT_NE(StopObj2, nullptr);
    EXPECT_EQ(StopObj2->NodeID(), 111);
    EXPECT_NE(StopObj2->NodeID(), 333);
    EXPECT_EQ(StopObj2->ID(), 1);

    // routes should behave regularly

    EXPECT_EQ(BusSystem.RouteCount(), 2);

    auto RouteA = BusSystem.RouteByName("A");
    ASSERT_NE(RouteA, nullptr);
    EXPECT_EQ(RouteA->GetStopID(0), 1);

    auto RouteB = BusSystem.RouteByName("B");
    ASSERT_NE(RouteB, nullptr);
    EXPECT_EQ(RouteB->GetStopID(0), 1);
}