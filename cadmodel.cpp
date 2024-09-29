#include "cadmodel.h"
#include <fstream>
#include <iostream>
#include <sstream>

std::string read_file(const std::string& filename) {
    std::ifstream ifs(filename.c_str());
    
    if(!ifs.is_open()) {
        throw CadmodelError("Cannot open file: " + filename);
    }
    
    std::string content((std::istreambuf_iterator<char>(ifs)),
            (std::istreambuf_iterator<char>()));

    ifs.close();
    return content;
}

std::vector<std::string> split(const std::string& text)
{
    std::vector<std::string> lines;
    std::string line;
    std::istringstream iss(text);
    while(std::getline(iss, line)) {
        lines.push_back(line);
    }
    return lines;
}

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
}
