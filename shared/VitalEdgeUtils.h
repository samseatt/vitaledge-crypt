#ifndef VITALEDGE_UTILS_H
#define VITALEDGE_UTILS_H

#include <string>

class VitalEdgeUtils {
public:
    static std::string base64Encode(const std::string& data);
    static std::string base64Decode(const std::string& base64Data);
};

#endif
