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
    lines_ = split(text);
    if (lines_.size() < 2) {
        throw CadmodelError("Not enough lines in text");
    }

    validate_1line(lines_[0]);
    validate_lastline(lines_[lines_.size() - 1]);

    read_facets();

}

void Cadmodel::validate_1line(const std::string& line)
{
    auto line1_tokens = tokenize(line);
    if (line1_tokens.size() < 2) {
        throw CadmodelError("Not enough tokens in line 1");
    }
    if (line1_tokens[0] != "solid") {
        throw CadmodelError("Invalid first token in line 1");
    }
}
    
void Cadmodel::validate_lastline(const std::string& line)
{
    auto last_line_tokens = tokenize(line);
    if (last_line_tokens[0] != "endsolid") {
        throw CadmodelError("Invalid last line");
    }
}
    
void Cadmodel::read_facets()
{
    index_ = 2;
    while (index_ < lines_.size() - 1) {
        auto facet = read_facet();
        facets_.push_back(facet);
    }
}

Facet Cadmodel::read_facet()
{
    Facet facet;

    facet.normal() = read_facet_normal();

    return facet;
}

Point Cadmodel::read_facet_normal()
{
    auto tokens = get_line_tokens();
    if(tokens.size() != 5) {
        throw CadmodelError("Invalid facet normal");
    }
    
    if (tokens[0] != "facet") {
        throw CadmodelError("Invalid facet normal");
    }

    if (tokens[1] != "normal") {
        throw CadmodelError("Invalid facet normal");
    }

    double x = std::stod(tokens[2]);
    double y = std::stod(tokens[3]);
    double z = std::stod(tokens[4]);
    Point normal(x, y, z);
    return normal;
}

std::vector<std::string> Cadmodel::get_line_tokens()
{
    if (index_ >= lines_.size()) {
        throw CadmodelError("End of file");
    }
    auto line_tokens = tokenize(lines_[index_]);
    index_++;
    return line_tokens;
}
