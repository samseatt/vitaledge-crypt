#include "VitalEdgeCrypto.h"
#include <drogon/drogon.h>
#include <drogon/utils/Utilities.h>  // Include Drogon's utility header

using namespace drogon;

int main() {
    // Route: /encrypt (AES Encryption)
    app().registerHandler("/encrypt",
        [](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
            auto json = req->getJsonObject();
            if (!json || !json->isMember("data") || !json->isMember("key") || !json->isMember("iv")) {
                Json::Value jsonResp;
                jsonResp["error"] = "Invalid request: 'data', 'key', and 'iv' are required.";
                auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
                resp->setStatusCode(HttpStatusCode::k400BadRequest);
                callback(resp);
                return;
            }

            try {
                std::string encrypted = VitalEdgeCrypto::encryptAES(
                    (*json)["data"].asString(),
                    (*json)["key"].asString(),
                    (*json)["iv"].asString()
                );
                Json::Value jsonResp;
                jsonResp["encrypted"] = encrypted;
                auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
                callback(resp);
            } catch (const std::exception& e) {
                Json::Value jsonResp;
                jsonResp["error"] = e.what();
                auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
                resp->setStatusCode(HttpStatusCode::k500InternalServerError);
                callback(resp);
            }
        },
        {Post});

    // Route: /decrypt (AES Decryption)
    // app().registerHandler("/decrypt",
    //     [](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    //         auto json = req->getJsonObject();
    //         if (!json || !json->isMember("data") || !json->isMember("key") || !json->isMember("iv")) {
    //             Json::Value jsonResp;
    //             jsonResp["error"] = "Invalid request: 'data', 'key', and 'iv' are required.";
    //             auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
    //             resp->setStatusCode(HttpStatusCode::k400BadRequest);
    //             callback(resp);
    //             return;
    //         }

    //         try {
    //             std::string decrypted = VitalEdgeCrypto::decryptAES(
    //                 (*json)["data"].asString(),
    //                 (*json)["key"].asString(),
    //                 (*json)["iv"].asString()
    //             );
    //             Json::Value jsonResp;
    //             jsonResp["decrypted"] = decrypted;
    //             auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
    //             callback(resp);
    //         } catch (const std::exception& e) {
    //             Json::Value jsonResp;
    //             jsonResp["error"] = e.what();
    //             auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
    //             resp->setStatusCode(HttpStatusCode::k500InternalServerError);
    //             callback(resp);
    //         }
    //     },
    //     {Post});
app().registerHandler("/decrypt",
    [](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
        auto json = req->getJsonObject();
        if (!json || !json->isMember("data") || !json->isMember("key") || !json->isMember("iv")) {
            Json::Value jsonResp;
            jsonResp["error"] = "Invalid request: 'data', 'key', and 'iv' are required.";
            auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
            resp->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(resp);
            return;
        }

        try {
            // Use Drogon's base64Decode
            std::string ciphertext = drogon::utils::base64Decode((*json)["data"].asString());

            // Perform AES decryption
            std::string plaintext = VitalEdgeCrypto::decryptAES(
                ciphertext,
                (*json)["key"].asString(),
                (*json)["iv"].asString()
            );

            Json::Value jsonResp;
            jsonResp["decrypted"] = plaintext;
            auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
            callback(resp);
        } catch (const std::exception& e) {
            Json::Value jsonResp;
            jsonResp["error"] = e.what();
            auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
            resp->setStatusCode(HttpStatusCode::k500InternalServerError);
            callback(resp);
        }
    },
    {Post});

    // Route: /obfuscate
    app().registerHandler("/obfuscate",
        [](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
            auto json = req->getJsonObject();
            if (!json || !json->isMember("data")) {
                Json::Value jsonResp;
                jsonResp["error"] = "Invalid request: 'data' is required.";
                auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
                resp->setStatusCode(HttpStatusCode::k400BadRequest);
                callback(resp);
                return;
            }

            std::string obfuscated = VitalEdgeCrypto::obfuscate((*json)["data"].asString());
            Json::Value jsonResp;
            jsonResp["obfuscated"] = obfuscated;
            auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
            callback(resp);
        },
        {Post});

    // Route: /deobfuscate
    app().registerHandler("/deobfuscate",
        [](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
            auto json = req->getJsonObject();
            if (!json || !json->isMember("data")) {
                Json::Value jsonResp;
                jsonResp["error"] = "Invalid request: 'data' is required.";
                auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
                resp->setStatusCode(HttpStatusCode::k400BadRequest);
                callback(resp);
                return;
            }

            std::string deobfuscated = VitalEdgeCrypto::deobfuscate((*json)["data"].asString());
            Json::Value jsonResp;
            jsonResp["deobfuscated"] = deobfuscated;
            auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
            callback(resp);
        },
        {Post});

    // Run the Drogon application
    app().addListener("0.0.0.0", 8084).run();

    return 0;
}
