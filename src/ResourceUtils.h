#ifndef RESOURCE_UTILS_H
#define RESOURCE_UTILS_H

#include <vector>
#include <string>

class ResourceUtils
{
public:
    static std::vector<char> readFile(const std::string& filename);
};

#endif
