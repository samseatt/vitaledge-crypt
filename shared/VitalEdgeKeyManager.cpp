#include "VitalEdgeKeyManager.h"
#include <openssl/rand.h>
#include <stdexcept>

std::string VitalEdgeKeyManager::generateKey(const std::string& algorithm) {
    if (algorithm != "AES") {
        throw std::invalid_argument("Unsupported algorithm");
    }
    std::string key(16, '\0'); // 16 bytes for AES-128
    RAND_bytes(reinterpret_cast<unsigned char*>(&key[0]), key.size());
    return key;
}
