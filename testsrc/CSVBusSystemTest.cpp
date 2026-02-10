#include <gtest/gtest.h>
#include "CSVBusSystem.h"
#include "StringDataSource.h"

TEST(CSVBusSystem, SimpleFiles){
    auto StopDataSource = std::make_shared < CStringDataSource >("stop_id,node_id\n"
                                                                   "1,123\n"
                                                                   "2,124");
    auto StopReader = std::make_shared < CDSVReader > (StopDataSource);
    auto RouteDataSource = std::make_shared < CStringDataSource >("route,stop_id\n"
                                                                  "A,1\n"
                                                                  "A,2");
    auto RouteReader = std::make_shared < CDSVReader > (RouteDataSource);

    CCSVBusSystem BusSystem(StopReader, RouteReader);



    EXPECT_EQ(BusSystem.StopCount(), 2);

    auto StopObj0 = BusSystem.StopByIndex(0);
    EXPECT_NE(StopObj0,nullptr);

    auto StopObj1 = BusSystem.StopByIndex(1);
    EXPECT_NE(StopObj1,nullptr);

    auto StopObjID1 = BusSystem.StopByID(1);
    ASSERT_NE(StopObjID1, nullptr);
    EXPECT_EQ(StopObjID1->ID(), 1);
    EXPECT_EQ(StopObjID1->NodeID(), 123);

    auto StopObjID2 = BusSystem.StopByID(2);
    ASSERT_NE(StopObjID2, nullptr);
    EXPECT_EQ(StopObjID2->ID(), 2);
    EXPECT_EQ(StopObjID2->NodeID(), 124);

}