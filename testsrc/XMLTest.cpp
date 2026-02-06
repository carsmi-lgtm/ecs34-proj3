#include <gtest/gtest.h>
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringDataSink.h"
#include "StringDataSource.h"

TEST(XMLReader, InitConstruction){
    auto src = std::make_shared<CStringDataSource>("<hello></hello>");
    CXMLReader reader(src);

    EXPECT_FALSE(reader.End());
}

TEST(XMLReader, TestReadEntity){
    auto src = std::make_shared<CStringDataSource>("<hello></hello>");
    CXMLReader reader(src);

    SXMLEntity entity;
    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "hello");

    ASSERT_TRUE(reader.ReadEntity(entity,false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "hello");
}

TEST(XMLReader, ReadEntitySkipCData){
    auto src = std::make_shared<CStringDataSource>("<hello>world</hello>");
    CXMLReader reader(src);

    SXMLEntity entity;
    ASSERT_TRUE(reader.ReadEntity(entity, true));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "hello"); 
    ASSERT_TRUE(reader.ReadEntity(entity, true));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "hello");
}

TEST(XMLReader, ReadEntityIncludesCData){
    auto src = std::make_shared<CStringDataSource>("<hello>world</hello>");
    CXMLReader reader(src);

    SXMLEntity entity;

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "world");
    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
}

TEST(XMLReader, ReadEntityWithAttributes){
    auto src = std::make_shared<CStringDataSource>("<hello a=\"1\" b=\"2\"></hello>");
    CXMLReader reader(src);

    SXMLEntity entity;
    ASSERT_TRUE(reader.ReadEntity(entity, false));

    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "hello");
    ASSERT_EQ(entity.DAttributes.size(), 2);
    EXPECT_EQ(entity.DAttributes[0].first, "a");
    EXPECT_EQ(entity.DAttributes[0].second, "1");
    EXPECT_EQ(entity.DAttributes[1].first, "b");
    EXPECT_EQ(entity.DAttributes[1].second, "2");
}

TEST(XMLWriter, InitConstruction){
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    EXPECT_EQ(sink->String(), "");
}

TEST(XMLWriter, StartAndEndElement){
    auto sink  = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    SXMLEntity start;
    start.DType = SXMLEntity::EType::StartElement;
    start.DNameData = "hello";

    SXMLEntity end;
    end.DType = SXMLEntity::EType::EndElement;
    end.DNameData = "hello";

    ASSERT_TRUE(writer.WriteEntity(start));
    ASSERT_TRUE(writer.WriteEntity(end));
    EXPECT_EQ(sink->String(), "<hello></hello>");
}

TEST(XMLWriter, CharDataInsideElement){
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    SXMLEntity start;
    start.DType = SXMLEntity::EType::StartElement;
    start.DNameData = "hello";

    SXMLEntity text;
    text.DType = SXMLEntity::EType::CharData;
    text.DNameData = "world";

    SXMLEntity end;
    end.DType = SXMLEntity::EType::EndElement;
    end.DNameData = "hello";

    ASSERT_TRUE(writer.WriteEntity(start));
    ASSERT_TRUE(writer.WriteEntity(text));
    ASSERT_TRUE(writer.WriteEntity(end));
    EXPECT_EQ(sink->String(), "<hello>world</hello>");
}

TEST(XMLWriter, FlushClosesNestedOpenTags){
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    SXMLEntity a;
    a.DType = SXMLEntity::EType::StartElement;
    a.DNameData = "a";

    SXMLEntity b;
    b.DType = SXMLEntity::EType::StartElement;
    b.DNameData = "b";

    ASSERT_TRUE(writer.WriteEntity(a));
    ASSERT_TRUE(writer.WriteEntity(b));
    ASSERT_TRUE(writer.Flush());
    EXPECT_EQ(sink->String(), "<a><b></b></a>");
}

TEST(XMLWriter, EndElementMismatchFails){
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    SXMLEntity start;
    start.DType = SXMLEntity::EType::StartElement;
    start.DNameData = "hello";

    SXMLEntity wrongend;
    wrongend.DType = SXMLEntity::EType::EndElement;
    wrongend.DNameData = "nope";
    ASSERT_TRUE(writer.WriteEntity(start));
    EXPECT_FALSE(writer.WriteEntity(wrongend));
}

TEST(XMLWriter, EmptyNameFails){
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);
    SXMLEntity start;
    start.DType = SXMLEntity::EType::StartElement;
    start.DNameData = "";
    EXPECT_FALSE(writer.WriteEntity(start));
}

TEST(XMLWriter, CompleteElementWritesSelfClosing){
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    SXMLEntity e;
    e.DType = SXMLEntity::EType::CompleteElement;
    e.DNameData = "hello";
    ASSERT_TRUE(writer.WriteEntity(e));
    EXPECT_EQ(sink->String(), "<hello/>");
}

TEST(XMLWriter, EndWithoutStartFails){
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    SXMLEntity end;
    end.DType = SXMLEntity::EType::EndElement;
    end.DNameData = "hello";
    EXPECT_FALSE(writer.WriteEntity(end));
}

TEST(XMLWriter, MismatchedEndFails){
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    SXMLEntity start;
    start.DType = SXMLEntity::EType::StartElement;
    start.DNameData = "hello";

    SXMLEntity wrongEnd;
    wrongEnd.DType = SXMLEntity::EType::EndElement;
    wrongEnd.DNameData = "bye";
    ASSERT_TRUE(writer.WriteEntity(start));
    EXPECT_FALSE(writer.WriteEntity(wrongEnd)); 
} 

TEST(XMLWriter, Flush){
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    SXMLEntity start;
    start.DType = SXMLEntity::EType::StartElement;
    start.DNameData = "hello";
    ASSERT_TRUE(writer.WriteEntity(start));
    ASSERT_TRUE(writer.Flush());
    EXPECT_EQ(sink->String(), "<hello></hello>");
}