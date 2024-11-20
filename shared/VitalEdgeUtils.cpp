#include "VitalEdgeUtils.h"
#include <openssl/bio.h>
#include <openssl/evp.h>

std::string VitalEdgeUtils::base64Encode(const std::string& data) {
    BIO* bio = BIO_new(BIO_s_mem());
    BIO* b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    BIO_write(bio, data.data(), data.size());
    BIO_flush(bio);

    char* encoded;
    long length = BIO_get_mem_data(bio, &encoded);
    std::string result(encoded, length);

    BIO_free_all(bio);
    return result;
}

std::string VitalEdgeUtils::base64Decode(const std::string& base64Data) {
    BIO* bio = BIO_new_mem_buf(base64Data.data(), base64Data.size());
    BIO* b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    std::vector<char> decoded(base64Data.size());
    int length = BIO_read(bio, decoded.data(), base64Data.size());

    BIO_free_all(bio);
    return std::string(decoded.data(), length);
}
