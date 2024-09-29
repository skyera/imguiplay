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

std::vector<std::string> tokenize(const std::string& str) {
    std::istringstream iss(str);
    std::string token;
    std::vector<std::string> tokens;

    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
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

    auto text = read_file(filename);
    auto lines = split(text);
    if (lines.size() < 2) {
        throw CadmodelError("Not enough lines in file: " + filename);
    }
}
    
void Cadmodel::read(const std::string& text)
{
    auto lines = split(text);
    if (lines.size() < 2) {
        throw CadmodelError("Not enough lines in text");
    }
    auto line1_tokens = tokenize(lines[0]);
    if (line1_tokens.size() < 2) {
        throw CadmodelError("Not enough tokens in line 1");
    }
}
