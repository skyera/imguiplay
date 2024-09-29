#ifndef CAD_MODEL_H
#define CAD_MODEL_H

#include <string>
#include <vector>

class Cadmodel
{
public:
    Cadmodel();
    ~Cadmodel();

    bool open(const std::string& filename);
};
#endif
