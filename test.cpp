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

TEST_CASE("read_first_line") {
    Cadmodel model;
    std::string filename = "data/rect.stl";
    model.open(filename);
}
