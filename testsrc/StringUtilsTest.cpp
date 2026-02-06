#include <gtest/gtest.h>
#include "StringUtils.h"
#include <vector>
#include <string>

TEST(StringUtilsTest, SliceTest){
    EXPECT_EQ(StringUtils::Slice("",0,5), std::string(""));
    EXPECT_EQ(StringUtils::Slice("HelloWorld", 4, 2), std::string(""));
    EXPECT_EQ(StringUtils::Slice("HelloWorld", 3, 3), std::string(""));
    EXPECT_EQ(StringUtils::Slice("HelloWorld", 0, 5), std::string("Hello"));
    EXPECT_EQ(StringUtils::Slice("HelloWorld", -5, 8), std::string("Wor"));
    EXPECT_EQ(StringUtils::Slice("HelloWorld", 6), std::string("orld"));
    EXPECT_EQ(StringUtils::Slice("HelloWorld", -13, 2), std::string("He"));
    EXPECT_EQ(StringUtils::Slice("HelloWorld", 4, 15), std::string("oWorld"));
    EXPECT_EQ(StringUtils::Slice("a",0,1), std::string("a"));
    EXPECT_EQ(StringUtils::Slice("HelloWorld", 5, 3), std::string(""));
}

TEST(StringUtilsTest, Capitalize){
    EXPECT_EQ(StringUtils::Capitalize(""),std::string(""));
    EXPECT_EQ(StringUtils::Capitalize("a"),std::string("A"));
    EXPECT_EQ(StringUtils::Capitalize("hello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::Capitalize("Hello"),std::string("Hello"));
    EXPECT_EQ(StringUtils::Capitalize("hELlO"),std::string("HELlO"));
    EXPECT_EQ(StringUtils::Capitalize("0hello"),std::string("0hello"));
}

TEST(StringUtilsTest, Upper){
    EXPECT_EQ(StringUtils::Upper(""),std::string(""));
    EXPECT_EQ(StringUtils::Upper("hElLo"),std::string("HELLO"));
    EXPECT_EQ(StringUtils::Upper("HELLO"),std::string("HELLO"));
    EXPECT_EQ(StringUtils::Upper("hello"),std::string("HELLO"));
    EXPECT_EQ(StringUtils::Upper("hello12$a"),std::string("HELLO12$A"));
}

TEST(StringUtilsTest, Lower){
    EXPECT_EQ(StringUtils::Lower(""),std::string(""));
    EXPECT_EQ(StringUtils::Lower("HeLlO"),std::string("hello"));
    EXPECT_EQ(StringUtils::Lower("hello"),std::string("hello"));
    EXPECT_EQ(StringUtils::Lower("HELLO"),std::string("hello"));
    EXPECT_EQ(StringUtils::Lower("HELLO12$A"),std::string("hello12$a"));
}

TEST(StringUtilsTest, LStrip){
    EXPECT_EQ(StringUtils::LStrip(""),std::string(""));
    EXPECT_EQ(StringUtils::LStrip("  "),std::string(""));
    EXPECT_EQ(StringUtils::LStrip("   hello"),std::string("hello"));
    EXPECT_EQ(StringUtils::LStrip("hello"),std::string("hello"));
    EXPECT_EQ(StringUtils::LStrip("hello    "),std::string("hello    "));
    EXPECT_EQ(StringUtils::LStrip(" hello  "),std::string("hello  "));
    EXPECT_EQ(StringUtils::LStrip("he llo"),std::string("he llo"));
    EXPECT_EQ(StringUtils::LStrip("\t\n hello\n\t"),std::string("hello\n\t"));
}

TEST(StringUtilsTest, RStrip){
    EXPECT_EQ(StringUtils::RStrip(""),std::string(""));
    EXPECT_EQ(StringUtils::RStrip("  "),std::string(""));
    EXPECT_EQ(StringUtils::RStrip("   hello"),std::string("   hello"));
    EXPECT_EQ(StringUtils::RStrip("hello"),std::string("hello"));
    EXPECT_EQ(StringUtils::RStrip("hello    "),std::string("hello"));
    EXPECT_EQ(StringUtils::RStrip(" hello  "),std::string(" hello"));
    EXPECT_EQ(StringUtils::RStrip("he llo"),std::string("he llo"));
    EXPECT_EQ(StringUtils::RStrip("\t\n hello\n\t"),std::string("\t\n hello"));
}

TEST(StringUtilsTest, Strip){
    EXPECT_EQ(StringUtils::Strip(""),std::string(""));
    EXPECT_EQ(StringUtils::Strip("  "),std::string(""));
    EXPECT_EQ(StringUtils::Strip("   hello"),std::string("hello"));
    EXPECT_EQ(StringUtils::Strip("hello"),std::string("hello"));
    EXPECT_EQ(StringUtils::Strip("hello    "),std::string("hello"));
    EXPECT_EQ(StringUtils::Strip(" hello  "),std::string("hello"));  
    EXPECT_EQ(StringUtils::Strip("he llo"),std::string("he llo"));
    EXPECT_EQ(StringUtils::Strip("\t\n hello\n\t"),std::string("hello"));
}

TEST(StringUtilsTest, Center){
    EXPECT_EQ(StringUtils::Center("Hello!", 5),std::string("Hello!"));
    EXPECT_EQ(StringUtils::Center("Hello!", 0),std::string("Hello!"));
    EXPECT_EQ(StringUtils::Center("Hello!", 10),std::string("  Hello!  "));
    EXPECT_EQ(StringUtils::Center("Hello!", 11),std::string("  Hello!   "));
}

TEST(StringUtilsTest, LJust){
    EXPECT_EQ(StringUtils::LJust("Hello!", 5),std::string("Hello!"));
    EXPECT_EQ(StringUtils::LJust("Hello!", 0),std::string("Hello!"));
    EXPECT_EQ(StringUtils::LJust("Hello!", 10),std::string("Hello!    "));
    EXPECT_EQ(StringUtils::LJust("Hello!", 11),std::string("Hello!     "));
}

TEST(StringUtilsTest, RJust){
    EXPECT_EQ(StringUtils::RJust("Hello!", 5),std::string("Hello!"));
    EXPECT_EQ(StringUtils::RJust("Hello!", 0),std::string("Hello!"));
    EXPECT_EQ(StringUtils::RJust("Hello!", 10),std::string("    Hello!"));
    EXPECT_EQ(StringUtils::RJust("Hello!", 11),std::string("     Hello!"));
}

TEST(StringUtilsTest, Replace){
    EXPECT_EQ(StringUtils::Replace("HelloWorld", "l", ""),std::string("HeoWord"));
    EXPECT_EQ(StringUtils::Replace("HelloWorld", "a", "b"),std::string("HelloWorld"));
    EXPECT_EQ(StringUtils::Replace("HelloWorld", "l", "l"),std::string("HelloWorld"));
    EXPECT_EQ(StringUtils::Replace("HelloWorld", "Hello", "World"),std::string("WorldWorld"));
    EXPECT_EQ(StringUtils::Replace("HelloWorld", "o", "x"),std::string("HellxWxrld"));
    EXPECT_EQ(StringUtils::Replace("HelloWorld", "", "x"),std::string("HelloWorld"));
}

TEST(StringUtilsTest, Split){

    std::vector<std::string> v1 = {"Hello","World"};
    EXPECT_EQ(StringUtils::Split("Hello World", " "),v1);

    std::vector<std::string> v2 = {"Hello","World", ""};
    EXPECT_EQ(StringUtils::Split("Hello,World,", ","),v2);

    std::vector<std::string> v3 = {"","","Hello","","World"};
    EXPECT_EQ(StringUtils::Split("  Hello  World", " "),v3);

    std::vector<std::string> v4 = {"Hello","World"};
    EXPECT_EQ(StringUtils::Split("  Hello World ", ""),v4);

    std::vector<std::string> v5 = {};
    EXPECT_EQ(StringUtils::Split("", ""), v5);
    
    std::vector<std::string> v6 = {"Hello World"};
    EXPECT_EQ(StringUtils::Split("Hello World", ","), v6);
    
    std::vector<std::string> v7 = {"","","",""};
    EXPECT_EQ(StringUtils::Split("---", "-"), v7);

}

TEST(StringUtilsTest, Join){
    std::vector<std::string> v1 = {"Hello","World"};
    EXPECT_EQ(StringUtils::Join(" ",v1), "Hello World");

    std::vector<std::string> v2 = {"OneItem"};
    EXPECT_EQ(StringUtils::Join(" ",v2), "OneItem");

    std::vector<std::string> v3 = {"one", "two", "three"};
    EXPECT_EQ(StringUtils::Join(",",v3),"one,two,three");
    
    EXPECT_EQ(StringUtils::Join("",v3),"onetwothree");

    std::vector<std::string> v4 = {};
    EXPECT_EQ(StringUtils::Join("",v4),"");

    std::vector<std::string> v5 = {"", "", ""};
    EXPECT_EQ(StringUtils::Join(",",v5),",,");

    std::vector<std::string> v6 = {""};
    EXPECT_EQ(StringUtils::Join(",",v6),"");

}

TEST(StringUtilsTest, ExpandTabs){
    EXPECT_EQ(StringUtils::ExpandTabs("",4), "");
    EXPECT_EQ(StringUtils::ExpandTabs("\t",4), "    ");
    EXPECT_EQ(StringUtils::ExpandTabs("\t\t\t",3), "         ");
    EXPECT_EQ(StringUtils::ExpandTabs("Hello\tWorld",5), "Hello     World");
    EXPECT_EQ(StringUtils::ExpandTabs("\tHelloWorld",4), "    HelloWorld");
    EXPECT_EQ(StringUtils::ExpandTabs("HelloWorld\t",4), "HelloWorld    ");
}

TEST(StringUtilsTest, EditDistance){
    EXPECT_EQ(StringUtils::EditDistance("","",true),0);
    EXPECT_EQ(StringUtils::EditDistance("str","",true),3);
    EXPECT_EQ(StringUtils::EditDistance("","string",true),6);
    EXPECT_EQ(StringUtils::EditDistance("str","str",true),0);
    EXPECT_EQ(StringUtils::EditDistance("str","word",true),3);
    EXPECT_EQ(StringUtils::EditDistance("str","string",true),3);
    EXPECT_EQ(StringUtils::EditDistance("strings","str",true),4);
    EXPECT_EQ(StringUtils::EditDistance("str","",true),3);
    EXPECT_EQ(StringUtils::EditDistance("horse","hone",true),2);
    EXPECT_EQ(StringUtils::EditDistance("editdistancetesting","letstesteditdistance",true),14);
    EXPECT_EQ(StringUtils::EditDistance("HOrse","hone",true),2);
    EXPECT_EQ(StringUtils::EditDistance("HOrse","hone",false),4);
}
