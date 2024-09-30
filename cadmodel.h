#ifndef CAD_MODEL_H
#define CAD_MODEL_H

#include <string>
#include <vector>
#include <exception>

std::string read_file(const std::string& filename);
std::vector<std::string> split(const std::string& text);
std::vector<std::string> tokenize(const std::string& str);


class Point
{
public:
    Point(double x=0.0, double y=0.0, double z=0.0) : x_(x), y_(y), z_(z) {}
private:
    double x_ = 0.0;
    double y_ = 0.0;
    double z_ = 0.0;
};

class Facet
{
public:
    Facet() {}
    ~Facet() {}

    const Point& normal() const { return normal_; }
    Point& normal() { return normal_; }
    std::vector<Point> points() { return points_; }
private:
    Point normal_;
    std::vector<Point> points_;
};

class CadmodelError: std::exception
{
public:
    CadmodelError(const std::string& what) : m_what(what) {}
    virtual ~CadmodelError() throw() {}
    virtual const char* what() const throw() { return m_what.c_str(); }
private:
    std::string m_what;
};

class Cadmodel
{
public:
    Cadmodel();
    ~Cadmodel();

    void open(const std::string& filename);
    void read(const std::string& text);
private:
    void validate_1line(const std::string& line); 
    void validate_lastline(const std::string& line);
    void read_facets();
    Facet read_facet();
    Point read_facet_normal();
    std::vector<std::string> get_line_tokens();

    
    std::vector<std::string> lines_;
    int index_ = 0;
    std::vector<Facet> facets_;
};
#endif
