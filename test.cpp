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

TEST_CASE("tokensize") {
    SUBCASE("empty_str") {
        std::string str = "";
        auto tokens = tokensize(str);
        REQUIRE(tokens.size() == 0);
    }
    SUBCASE("empty_spaces") {
        std::string str = "  ";
        auto tokens = tokensize(str);
        REQUIRE(tokens.size() == 0);
    }
    SUBCASE("str_with_spaces") {
        std::string str = "1 2 3";
        auto tokens = tokensize(str);
        REQUIRE(tokens.size() == 3);
        REQUIRE(tokens[0] == "1");
        REQUIRE(tokens[1] == "2");
        REQUIRE(tokens[2] == "3");
    }
    SUBCASE("normal2") {
        std::string str = " 1   2    3 ";    
        auto tokens = tokensize(str);
        REQUIRE(tokens.size() == 3);
        REQUIRE(tokens[0] == "1");
        REQUIRE(tokens[1] == "2");
        REQUIRE(tokens[2] == "3");
    }
}

TEST_CASE("read") {
    Cadmodel model;

    SUBCASE("empty line") {
        std::string text = "";
        REQUIRE_THROWS_AS(model.read(text), CadmodelError);
    }
    SUBCASE("2line_emptyline") {
        std::string text = "line1\nline2";
        REQUIRE_THROWS_AS(model.read(text), CadmodelError);
    }
}
