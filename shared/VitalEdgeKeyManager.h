#ifndef VITALEDGE_KEYMANAGER_H
#define VITALEDGE_KEYMANAGER_H

#include <string>

class VitalEdgeKeyManager {
public:
    static std::string generateKey(const std::string& algorithm);
};

#endif
