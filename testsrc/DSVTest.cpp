#include <gtest/gtest.h>
#include "DSVReader.h"
#include "DSVWriter.h"
#include "StringDataSink.h"
#include "StringDataSource.h"

// DSV READER TESTS

TEST(DSVReader, InitConstruction){
    auto src = std::make_shared< CStringDataSource >("Hello,World,!\n");
    CDSVReader reader1(src, ',');
    EXPECT_FALSE(reader1.End());
}

TEST(DSVReader, TestReadRow){
    auto src = std::make_shared< CStringDataSource >("Hello,World,!\n");
    CDSVReader reader1(src, ',');

    std::vector<std::string> row;
    ASSERT_TRUE(reader1.ReadRow(row));
    EXPECT_EQ(row[0], "Hello");
    EXPECT_EQ(row[1], "World");
    EXPECT_EQ(row[2], "!");
    EXPECT_EQ(row.size(), 3);
    
}

TEST(DSVReader, TestReadRow_emptyrow){
    auto src = std::make_shared< CStringDataSource >("\n");
    CDSVReader reader1(src, ',');

    std::vector<std::string> row;
    ASSERT_TRUE(reader1.ReadRow(row));
    EXPECT_TRUE(row.empty());
    EXPECT_EQ(row.size(), 0);
}

TEST(DSVReader, TestReadRow_emptycells){
    auto src = std::make_shared< CStringDataSource >(",\n");
    CDSVReader reader1(src, ',');

    std::vector<std::string> row;
    ASSERT_TRUE(reader1.ReadRow(row));
    EXPECT_EQ(row[0], "");
    EXPECT_EQ(row[1], "");
    EXPECT_EQ(row.size(), 2);
}

TEST(DSVReader, TestReadRow_doublequotedelim){
    auto src = std::make_shared< CStringDataSource >("Hello,World\n");
    CDSVReader reader1(src, '"');

    std::vector<std::string> row;
    ASSERT_TRUE(reader1.ReadRow(row));
    EXPECT_EQ(row[0], "Hello");
    EXPECT_EQ(row[1], "World");
    EXPECT_EQ(row.size(), 2);
}


TEST(DSVReader, TestReadRow_doublequoteincell){
    auto src = std::make_shared< CStringDataSource >("\"Say \"\"Hello\"\"\",World!\n");
    CDSVReader reader1(src, ',');

    std::vector<std::string> row;
    ASSERT_TRUE(reader1.ReadRow(row));
    EXPECT_EQ(row[0], "Say \"Hello\"");
    EXPECT_EQ(row[1], "World!");
    EXPECT_EQ(row.size(), 2);
}

TEST(DSVReader, TestReadRow_leadingdelim){
    auto src = std::make_shared< CStringDataSource >(",a,b\n");
    CDSVReader reader1(src, ',');

    std::vector<std::string> row;
    ASSERT_TRUE(reader1.ReadRow(row));
    EXPECT_EQ(row[0], "");
    EXPECT_EQ(row[1], "a");
    EXPECT_EQ(row[2], "b");
    EXPECT_EQ(row.size(), 3);
}

TEST(DSVReader, TestReadRow_trailingdelim){
    auto src = std::make_shared< CStringDataSource >("a,b,\n");
    CDSVReader reader1(src, ',');

    std::vector<std::string> row;
    ASSERT_TRUE(reader1.ReadRow(row));
    EXPECT_EQ(row[0], "a");
    EXPECT_EQ(row[1], "b");
    EXPECT_EQ(row[2], "");
    EXPECT_EQ(row.size(), 3);
}

TEST(DSVReader, TestReadRow_embeddednewline){
    auto src = std::make_shared< CStringDataSource >("\"a\nb\",c\n");
    CDSVReader reader1(src, ',');

    std::vector<std::string> row;
    ASSERT_TRUE(reader1.ReadRow(row));
    EXPECT_EQ(row[0], "a\nb");
    EXPECT_EQ(row[1], "c");
    EXPECT_EQ(row.size(), 2);
}

TEST(DSVReader, TestReadRow_embeddeddelim){
    auto src = std::make_shared< CStringDataSource >("\"a,b\",c\n");
    CDSVReader reader1(src, ',');

    std::vector<std::string> row;
    ASSERT_TRUE(reader1.ReadRow(row));
    EXPECT_EQ(row[0], "a,b");
    EXPECT_EQ(row[1], "c");
    EXPECT_EQ(row.size(), 2);
}

TEST(DSVReader, TestReadRow_emptysource){
    auto src = std::make_shared< CStringDataSource >("");
    CDSVReader reader1(src, ',');

    std::vector<std::string> row;
    EXPECT_FALSE(reader1.ReadRow(row));
    EXPECT_TRUE(row.empty());
}

TEST(DSVReader, End){
    auto src = std::make_shared< CStringDataSource >("Hello,World,!\n");
    CDSVReader reader1(src, ',');

    EXPECT_FALSE(reader1.End());
}


// DSV WRITER TESTS

TEST(DSVWriter, InitConstruction){
    auto sink = std::make_shared< CStringDataSink >();
    CDSVWriter writer1(sink, ',', false);

    EXPECT_EQ(sink->String(), "");
}

TEST(DSVWriter, TestWriteRow){
    auto sink = std::make_shared< CStringDataSink >();
    CDSVWriter writer1(sink, ',', false);

    std::vector<std::string> row = {"Hello", "World", "!"};
    ASSERT_TRUE(writer1.WriteRow(row));
    EXPECT_EQ(sink->String(), "Hello,World,!\n");
}

TEST(DSVWriter, TestWriteRow_emptyrow){
    auto sink = std::make_shared< CStringDataSink >();
    CDSVWriter writer1(sink, ',', false);

    std::vector<std::string> row = {};
    ASSERT_TRUE(writer1.WriteRow(row));
    EXPECT_EQ(sink->String(), "\n");
}

TEST(DSVWriter, TestWriteRow_doublequotedelim){
    auto sink = std::make_shared< CStringDataSink >();
    CDSVWriter writer1(sink, '"', false);

    std::vector<std::string> row = {"a", "b", "c", "d"};
    ASSERT_TRUE(writer1.WriteRow(row));
    EXPECT_EQ(sink->String(), "a,b,c,d\n");
}

TEST(DSVWriter, TestWriteRow_trailingdelim){
    auto sink = std::make_shared< CStringDataSink >();
    CDSVWriter writer1(sink, ',', false);

    std::vector<std::string> row = {"a", "b", ""};
    ASSERT_TRUE(writer1.WriteRow(row));
    EXPECT_EQ(sink->String(), "a,b,\n");
}

TEST(DSVWriter, TestWriteRow_leadingdelim){
    auto sink = std::make_shared< CStringDataSink >();
    CDSVWriter writer1(sink, ',', false);

    std::vector<std::string> row = {"","a", "b"};
    ASSERT_TRUE(writer1.WriteRow(row));
    EXPECT_EQ(sink->String(), ",a,b\n");
}

TEST(DSVWriter, TestWriteRow_doublequotesincells){
    auto sink = std::make_shared< CStringDataSink >();
    CDSVWriter writer1(sink, ',', false);

    std::vector<std::string> row = {"Say \"Hello\"", "World"};
    ASSERT_TRUE(writer1.WriteRow(row));
    EXPECT_EQ(sink->String(), "\"Say \"\"Hello\"\"\",World\n");
}

TEST(DSVWriter, TestWriteRow_embeddednewline){
    auto sink = std::make_shared< CStringDataSink >();
    CDSVWriter writer1(sink, ',', false);

    std::vector<std::string> row = {"a\nb", "c"};
    ASSERT_TRUE(writer1.WriteRow(row));
    EXPECT_EQ(sink->String(), "\"a\nb\",c\n");
}

TEST(DSVWriter, TestWriteRow_embeddeddelim){
    auto sink = std::make_shared< CStringDataSink >();
    CDSVWriter writer1(sink, ',', false);

    std::vector<std::string> row = {"a,b", "c"};
    ASSERT_TRUE(writer1.WriteRow(row));
    EXPECT_EQ(sink->String(), "\"a,b\",c\n");
}
