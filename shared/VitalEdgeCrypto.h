#ifndef VITALEDGE_CRYPTO_H
#define VITALEDGE_CRYPTO_H

#include <string>
#include <vector>

class VitalEdgeCrypto {
public:
    // Symmetric encryption (AES)
    static std::string encryptAES(const std::string& plaintext, const std::string& key, const std::string& iv);
    static std::string decryptAES(const std::string& ciphertext, const std::string& key, const std::string& iv);

    // Asymmetric encryption (RSA)
    static std::vector<uint8_t> encryptRSA(const std::string& plaintext, const std::string& publicKey);
    static std::string decryptRSA(const std::vector<uint8_t>& ciphertext, const std::string& privateKey);

    // Utility functions for key/IV generation
    static std::string generateRandomKey(size_t length);
    static std::string generateRandomIV(size_t length);

    // Obfuscation and De-Obfuscation
    static std::string obfuscate(const std::string& data);
    static std::string deobfuscate(const std::string& obfuscatedData);

    class KeyManager {
    public:
        static void saveKeyToFile(const std::string& key, const std::string& filepath);
        static std::string loadKeyFromFile(const std::string& filepath);
};


};

#endif // VITALEDGE_CRYPTO_H
