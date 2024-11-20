#include <gtest/gtest.h>
#include "VitalEdgeCrypto.h"
#include <fstream>
#include <sstream>

// Test AES encryption and decryption
TEST(VitalEdgeCryptoTest, AESEncryptionDecryption) {
    std::string plaintext = "Test AES Encryption";
    std::string key = VitalEdgeCrypto::generateRandomKey(32);
    std::string iv = VitalEdgeCrypto::generateRandomIV(16);

    std::string encrypted = VitalEdgeCrypto::encryptAES(plaintext, key, iv);
    std::string decrypted = VitalEdgeCrypto::decryptAES(encrypted, key, iv);

    EXPECT_EQ(plaintext, decrypted); // Ensure decrypted text matches original
}

// Test RSA encryption and decryption
// TEST(VitalEdgeCryptoTest, RSAEncryptionDecryption) {
//     std::string plaintext = "Test RSA Encryption";

//     // Generate RSA key pair
//     std::string publicKey =
//         "-----BEGIN PUBLIC KEY-----\n"
//         "...Your RSA Public Key...\n"
//         "-----END PUBLIC KEY-----\n";
//     std::string privateKey =
//         "-----BEGIN PRIVATE KEY-----\n"
//         "...Your RSA Private Key...\n"
//         "-----END PRIVATE KEY-----\n";

//     std::vector<uint8_t> encrypted = VitalEdgeCrypto::encryptRSA(plaintext, publicKey);
//     std::string decrypted = VitalEdgeCrypto::decryptRSA(encrypted, privateKey);

//     EXPECT_EQ(plaintext, decrypted); // Ensure decrypted text matches original
// }

// Helper function to read a key file
std::string readKeyFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open key file: " + filepath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Test RSA encryption and decryption
TEST(VitalEdgeCryptoTest, RSAEncryptionDecryption) {
    std::string plaintext = "Test RSA Encryption";

    // Read keys from files
    std::string publicKey = readKeyFromFile("public.pem");
    std::string privateKey = readKeyFromFile("private.pem");

    // Encrypt and decrypt
    std::vector<uint8_t> encrypted = VitalEdgeCrypto::encryptRSA(plaintext, publicKey);
    std::string decrypted = VitalEdgeCrypto::decryptRSA(encrypted, privateKey);

    // Validate
    EXPECT_EQ(plaintext, decrypted);
}

// Test random key generation
TEST(VitalEdgeCryptoTest, RandomKeyGeneration) {
    std::string key1 = VitalEdgeCrypto::generateRandomKey(32);
    std::string key2 = VitalEdgeCrypto::generateRandomKey(32);

    EXPECT_NE(key1, key2); // Random keys should not match
    EXPECT_EQ(key1.size(), 32); // Key size should match requested length
}

TEST(VitalEdgeCryptoTest, ObfuscationDeobfuscation) {
    std::string plaintext = "Test Obfuscation";

    std::string obfuscated = VitalEdgeCrypto::obfuscate(plaintext);
    std::string deobfuscated = VitalEdgeCrypto::deobfuscate(obfuscated);

    EXPECT_EQ(plaintext, deobfuscated);
}

TEST(KeyManagerTest, SaveAndLoadKey) {
    std::string key = "TestKey";
    std::string filepath = "test_key.txt";

    VitalEdgeCrypto::KeyManager::saveKeyToFile(key, filepath);
    std::string loadedKey = VitalEdgeCrypto::KeyManager::loadKeyFromFile(filepath);

    EXPECT_EQ(key, loadedKey);

    // Clean up
    std::remove(filepath.c_str());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
