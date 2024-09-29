#ifndef CAD_MODEL_H
#define CAD_MODEL_H

#include <string>
#include <vector>
#include <exception>

std::string read_file(const std::string& filename);
std::vector<std::string> split(const std::string& text);

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
private:
    
};
#endif
