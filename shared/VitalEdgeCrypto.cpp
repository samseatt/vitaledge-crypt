#include "VitalEdgeCrypto.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <sstream>

// Helper to handle OpenSSL errors
static void handleOpenSSLErrors() {
    unsigned long errCode;
    while ((errCode = ERR_get_error())) {
        char *err = ERR_error_string(errCode, nullptr);
        throw std::runtime_error(err);
    }
}

// Save key to file
void VitalEdgeCrypto::KeyManager::saveKeyToFile(const std::string& key, const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for writing: " + filepath);
    }
    file << key;
    file.close();
}

// Load key from file
std::string VitalEdgeCrypto::KeyManager::loadKeyFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for reading: " + filepath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Symmetric encryption (AES)
std::string VitalEdgeCrypto::encryptAES(const std::string& plaintext, const std::string& key, const std::string& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleOpenSSLErrors();

    const EVP_CIPHER* cipher = EVP_aes_256_cbc();
    if (!EVP_EncryptInit_ex(ctx, cipher, nullptr, (const unsigned char*)key.data(), (const unsigned char*)iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        handleOpenSSLErrors();
    }

    std::vector<unsigned char> ciphertext(plaintext.size() + EVP_CIPHER_block_size(cipher));
    int len = 0, ciphertextLen = 0;

    if (!EVP_EncryptUpdate(ctx, ciphertext.data(), &len, (const unsigned char*)plaintext.data(), plaintext.size())) {
        EVP_CIPHER_CTX_free(ctx);
        handleOpenSSLErrors();
    }
    ciphertextLen += len;

    if (!EVP_EncryptFinal_ex(ctx, ciphertext.data() + ciphertextLen, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        handleOpenSSLErrors();
    }
    ciphertextLen += len;

    EVP_CIPHER_CTX_free(ctx);
    return std::string(ciphertext.begin(), ciphertext.begin() + ciphertextLen);
}

std::string VitalEdgeCrypto::decryptAES(const std::string& ciphertext, const std::string& key, const std::string& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleOpenSSLErrors();

    const EVP_CIPHER* cipher = EVP_aes_256_cbc();
    if (!EVP_DecryptInit_ex(ctx, cipher, nullptr, (const unsigned char*)key.data(), (const unsigned char*)iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        handleOpenSSLErrors();
    }

    std::vector<unsigned char> plaintext(ciphertext.size());
    int len = 0, plaintextLen = 0;

    if (!EVP_DecryptUpdate(ctx, plaintext.data(), &len, (const unsigned char*)ciphertext.data(), ciphertext.size())) {
        EVP_CIPHER_CTX_free(ctx);
        handleOpenSSLErrors();
    }
    plaintextLen += len;

    if (!EVP_DecryptFinal_ex(ctx, plaintext.data() + plaintextLen, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        handleOpenSSLErrors();
    }
    plaintextLen += len;

    EVP_CIPHER_CTX_free(ctx);
    return std::string(plaintext.begin(), plaintext.begin() + plaintextLen);
}

// // Asymmetric encryption (RSA)
// std::vector<uint8_t> VitalEdgeCrypto::encryptRSA(const std::string& plaintext, const std::string& publicKey) {
//     BIO* keyBio = BIO_new_mem_buf(publicKey.data(), -1);
//     if (!keyBio) handleOpenSSLErrors();

//     RSA* rsa = PEM_read_bio_RSA_PUBKEY(keyBio, nullptr, nullptr, nullptr);
//     BIO_free(keyBio);
//     if (!rsa) handleOpenSSLErrors();

//     std::vector<uint8_t> ciphertext(RSA_size(rsa));
//     int result = RSA_public_encrypt(plaintext.size(), (const unsigned char*)plaintext.data(),
//                                      ciphertext.data(), rsa, RSA_PKCS1_OAEP_PADDING);

//     RSA_free(rsa);
//     if (result == -1) handleOpenSSLErrors();

//     return ciphertext;
// }

// std::string VitalEdgeCrypto::decryptRSA(const std::vector<uint8_t>& ciphertext, const std::string& privateKey) {
//     BIO* keyBio = BIO_new_mem_buf(privateKey.data(), -1);
//     if (!keyBio) handleOpenSSLErrors();

//     RSA* rsa = PEM_read_bio_RSAPrivateKey(keyBio, nullptr, nullptr, nullptr);
//     BIO_free(keyBio);
//     if (!rsa) handleOpenSSLErrors();

//     std::vector<uint8_t> plaintext(RSA_size(rsa));
//     int result = RSA_private_decrypt(ciphertext.size(), ciphertext.data(),
//                                      plaintext.data(), rsa, RSA_PKCS1_OAEP_PADDING);

//     RSA_free(rsa);
//     if (result == -1) handleOpenSSLErrors();

//     return std::string(plaintext.begin(), plaintext.begin() + result);
// }

// RSA Encryption
std::vector<uint8_t> VitalEdgeCrypto::encryptRSA(const std::string& plaintext, const std::string& publicKey) {
    BIO* keyBio = BIO_new_mem_buf(publicKey.data(), -1);
    if (!keyBio) handleOpenSSLErrors();

    EVP_PKEY* evpKey = PEM_read_bio_PUBKEY(keyBio, nullptr, nullptr, nullptr);
    BIO_free(keyBio);
    if (!evpKey) handleOpenSSLErrors();

    RSA* rsa = EVP_PKEY_get1_RSA(evpKey);
    EVP_PKEY_free(evpKey);
    if (!rsa) handleOpenSSLErrors();

    std::vector<uint8_t> ciphertext(RSA_size(rsa));
    int result = RSA_public_encrypt(plaintext.size(),
                                     (const unsigned char*)plaintext.data(),
                                     ciphertext.data(), rsa, RSA_PKCS1_OAEP_PADDING);

    RSA_free(rsa);
    if (result == -1) handleOpenSSLErrors();

    return ciphertext;
}

// RSA Decryption
std::string VitalEdgeCrypto::decryptRSA(const std::vector<uint8_t>& ciphertext, const std::string& privateKey) {
    BIO* keyBio = BIO_new_mem_buf(privateKey.data(), -1);
    if (!keyBio) handleOpenSSLErrors();

    EVP_PKEY* evpKey = PEM_read_bio_PrivateKey(keyBio, nullptr, nullptr, nullptr);
    BIO_free(keyBio);
    if (!evpKey) handleOpenSSLErrors();

    RSA* rsa = EVP_PKEY_get1_RSA(evpKey);
    EVP_PKEY_free(evpKey);
    if (!rsa) handleOpenSSLErrors();

    std::vector<uint8_t> plaintext(RSA_size(rsa));
    int result = RSA_private_decrypt(ciphertext.size(),
                                     ciphertext.data(),
                                     plaintext.data(), rsa, RSA_PKCS1_OAEP_PADDING);

    RSA_free(rsa);
    if (result == -1) handleOpenSSLErrors();

    return std::string(plaintext.begin(), plaintext.begin() + result);
}

// Utility: Generate random key
std::string VitalEdgeCrypto::generateRandomKey(size_t length) {
    std::string key(length, '\0');
    if (!RAND_bytes((unsigned char*)key.data(), length)) handleOpenSSLErrors();
    return key;
}

// Utility: Generate random IV
std::string VitalEdgeCrypto::generateRandomIV(size_t length) {
    return generateRandomKey(length); // Same as key generation
}

// Obfuscate (Base64 Encode)
std::string VitalEdgeCrypto::obfuscate(const std::string& data) {
    BIO* bio = BIO_new(BIO_s_mem());
    BIO* b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // No newlines in output
    BIO_write(bio, data.data(), data.size());
    BIO_flush(bio);

    char* encoded;
    long encodedLength = BIO_get_mem_data(bio, &encoded);
    std::string result(encoded, encodedLength);

    BIO_free_all(bio);
    return result;
}

// Deobfuscate (Base64 Decode)
std::string VitalEdgeCrypto::deobfuscate(const std::string& obfuscatedData) {
    BIO* bio = BIO_new_mem_buf(obfuscatedData.data(), obfuscatedData.size());
    BIO* b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // No newlines in input

    std::vector<char> buffer(obfuscatedData.size());
    int length = BIO_read(bio, buffer.data(), buffer.size());
    if (length < 0) handleOpenSSLErrors();

    BIO_free_all(bio);
    return std::string(buffer.begin(), buffer.begin() + length);
}
