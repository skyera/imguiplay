#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "cadmodel.h"

TEST_CASE("ctor") {
    Cadmodel model;    
}

TEST_CASE("open_non_existing_file") {
    Cadmodel model;
    std::string filename = "data/xrect.stl";
    REQUIRE_THROWS_AS(model.open(filename), CadmodelError);
}

TEST_CASE("split_text") {
    SUBCASE("2_line") {
        std::string str = "solid\nend";
        auto lines = split(str);
        REQUIRE(lines.size() == 2);
        REQUIRE(lines[0] == "solid");
        REQUIRE(lines[1] == "end");
    }
}

TEST_CASE("tokenize") {
    SUBCASE("empty_str") {
        std::string str = "";
        auto tokens = tokenize(str);
        REQUIRE(tokens.size() == 0);
    }
    SUBCASE("empty_spaces") {
        std::string str = "  ";
        auto tokens = tokenize(str);
        REQUIRE(tokens.size() == 0);
    }
    SUBCASE("str_with_spaces") {
        std::string str = "1 2 3";
        auto tokens = tokenize(str);
        REQUIRE(tokens.size() == 3);
        REQUIRE(tokens[0] == "1");
        REQUIRE(tokens[1] == "2");
        REQUIRE(tokens[2] == "3");
    }
    SUBCASE("normal2") {
        std::string str = " 1   2    3 ";    
        auto tokens = tokenize(str);
        REQUIRE(tokens.size() == 3);
        REQUIRE(tokens[0] == "1");
        REQUIRE(tokens[1] == "2");
        REQUIRE(tokens[2] == "3");
    }
    SUBCASE("solid_end") {
        std::string str = "solid\nend";
        auto lines = split(str);
        REQUIRE(lines.size() == 2);
        REQUIRE(lines[0] == "solid");
        REQUIRE(lines[1] == "end");
        auto tokens = tokenize(lines[0]);
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0] == "solid");
    }
}

TEST_CASE("read") {
    Cadmodel model;

    SUBCASE("empty line") {
        std::string text = "";
        REQUIRE_THROWS_AS(model.read(text), CadmodelError);
    }
    SUBCASE("invalid_firstline") {
        std::string text = "line1\nline2";
        REQUIRE_THROWS_AS(model.read(text), CadmodelError);
    }
}
