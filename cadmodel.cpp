#include "cadmodel.h"
#include <fstream>
#include <iostream>


Cadmodel::Cadmodel()
{

}
Cadmodel::~Cadmodel()
{

}

void Cadmodel::open(const std::string& filename)
{
    std::ifstream ifs(filename.c_str());
    if(!ifs.is_open()) {
        throw CadmodelError("Cannot open file: " + filename);
    }

    std::string line;
    while(std::getline(ifs, line)) {
        std::cout << line << std::endl;
    }
}
