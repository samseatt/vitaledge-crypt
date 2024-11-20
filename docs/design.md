# **Design Document for VitalEdge Crypt**


## **1. Overview**

**VitalEdge Crypt** is a C++ microservice designed to handle computationally intensive cryptographic operations for the VitalEdge ecosystem. Its responsibilities include encryption, decryption, and obfuscation of sensitive data transmitted between microservices, ensuring secure data handling at high performance.

This document outlines the architecture, design, interfaces, components, and frameworks for VitalEdge Crypt, establishing the foundation for its development and integration with other services.

---

## **2. Objectives**

- **High-Performance Cryptography**: Efficiently handle encryption and decryption of sensitive data.
- **Lightweight Obfuscation**: Provide a mechanism for lightweight data protection where encryption is not necessary.
- **Centralized Cryptographic Operations**: Serve as the single source for cryptographic functionalities, ensuring uniform security practices across the ecosystem.
- **Ease of Integration**: Expose a REST API for seamless interaction with other VitalEdge microservices (e.g., VitalEdge Security).

---

## **3. Responsibilities**

1. **Encryption**:
   - Provide symmetric and asymmetric encryption (AES, RSA).
   - Generate encryption keys if required for session-specific encryption.

2. **Decryption**:
   - Decrypt data previously encrypted using the service.
   - Support both symmetric and asymmetric decryption.

3. **Lightweight Obfuscation**:
   - Support reversible, low-overhead data obfuscation for cases requiring minimal security.

4. **Key Management**:
   - Manage encryption keys securely, supporting both static and dynamic key retrieval.
   - Integrate with secure key vaults if necessary.

5. **Logging**:
   - Record cryptographic operations for auditability.

---

## **4. Architecture**

### **Core Components**

1. **REST API Interface**:
   - Provides endpoints for encryption, decryption, and obfuscation.
   - Built using a lightweight C++ web framework such as **Crow** or **Restbed**.

2. **Crypto Engine**:
   - Implements cryptographic operations using a high-performance library like **OpenSSL**.
   - Encapsulates encryption, decryption, and obfuscation logic.

3. **Key Manager**:
   - Handles key generation, storage, and retrieval.
   - Ensures that keys are securely managed in memory.

4. **Audit Logger**:
   - Logs cryptographic operations to a file or external system (e.g., VitalEdge Blockchain for critical events).

---

## **5. Interfaces**

### **5.1. Ingress API**

#### **1. Encryption API**
- **Endpoint**: `/encrypt`
- **Method**: POST
- **Description**: Encrypts data using a specified algorithm and key.
- **Request**:
  ```json
  {
      "data": "Sensitive information",
      "algorithm": "AES",
      "key": "optional-base64-key"
  }
  ```
- **Response**:
  ```json
  {
      "encrypted_data": "Base64-encoded encrypted string",
      "key_id": "optional-key-id"
  }
  ```

#### **2. Decryption API**
- **Endpoint**: `/decrypt`
- **Method**: POST
- **Description**: Decrypts data using the provided key.
- **Request**:
  ```json
  {
      "encrypted_data": "Base64-encoded encrypted string",
      "key": "optional-base64-key"
  }
  ```
- **Response**:
  ```json
  {
      "data": "Decrypted information"
  }
  ```

#### **3. Obfuscation API**
- **Endpoint**: `/obfuscate`
- **Method**: POST
- **Description**: Obfuscates data for lightweight reversible protection.
- **Request**:
  ```json
  {
      "data": "Sensitive information"
  }
  ```
- **Response**:
  ```json
  {
      "obfuscated_data": "Base64-encoded obfuscated string"
  }
  ```

#### **4. De-Obfuscation API**
- **Endpoint**: `/deobfuscate`
- **Method**: POST
- **Description**: Reverses obfuscation to retrieve the original data.
- **Request**:
  ```json
  {
      "obfuscated_data": "Base64-encoded obfuscated string"
  }
  ```
- **Response**:
  ```json
  {
      "data": "Original information"
  }
  ```

---

### **5.2. Egress API (Interactions with Key Vaults)**
If dynamic key management is implemented, the service will interact with external key vaults (e.g., AWS KMS, HashiCorp Vault) to retrieve and manage keys.

#### Example Interaction:
- **Fetch Key**:
  - URL: `/fetch-key`
  - Response:
    ```json
    {
        "key_id": "key-12345",
        "key": "Base64-encoded key material"
    }
    ```

---

## **6. Class Design**

### **6.1. CryptoEngine**
Handles core cryptographic operations.

#### **Methods**:
- `std::string encrypt(const std::string& data, const std::string& key, const std::string& algorithm)`
- `std::string decrypt(const std::string& encryptedData, const std::string& key, const std::string& algorithm)`
- `std::string obfuscate(const std::string& data)`
- `std::string deobfuscate(const std::string& obfuscatedData)`

---

### **6.2. KeyManager**
Manages encryption keys.

#### **Methods**:
- `std::string generateKey(const std::string& algorithm)`
- `std::string fetchKey(const std::string& keyId)`
- `void storeKey(const std::string& keyId, const std::string& key)`

---

### **6.3. Logger**
Logs cryptographic operations.

#### **Methods**:
- `void logEvent(const std::string& eventType, const std::string& details)`

---

### **6.4. RestServer**
Implements the REST API interface using a web framework.

#### **Methods**:
- `void startServer()`
- `void handleEncryptRequest()`
- `void handleDecryptRequest()`
- `void handleObfuscateRequest()`
- `void handleDeobfuscateRequest()`

---

## **7. Frameworks and Libraries**

1. **C++ Web Framework**:
   - **Crow**: A modern, lightweight framework for RESTful APIs.
     - [GitHub](https://github.com/CrowCpp/Crow)
   - **Restbed**: A more feature-rich framework for REST APIs.
     - [GitHub](https://github.com/Corvusoft/restbed)

2. **Cryptographic Library**:
   - **OpenSSL**: Industry-standard cryptographic library for encryption, decryption, and key management.
     - [OpenSSL](https://www.openssl.org/)

3. **JSON Library**:
   - **nlohmann/json**: A modern C++ library for JSON parsing and serialization.
     - [GitHub](https://github.com/nlohmann/json)

4. **Logging Framework**:
   - **spdlog**: Fast and feature-rich logging library.
     - [GitHub](https://github.com/gabime/spdlog)

---

## **8. Deployment**

### **Docker Configuration**

#### **Dockerfile**:
```dockerfile
FROM gcc:latest
WORKDIR /app

# Install dependencies (example for Debian-based containers)
RUN apt-get update && apt-get install -y \
    libssl-dev \
    cmake \
    make

# Copy and build the application
COPY . .
RUN make

# Expose the service port
EXPOSE 8084

# Run the service
CMD ["./vitaledge-crypt"]
```

---

## **9. Security Considerations**

1. **Key Security**:
   - Store keys securely in memory and never log sensitive data.
2. **Rate Limiting**:
   - Enforce rate limits on APIs to prevent abuse.
3. **TLS Encryption**:
   - Use HTTPS for all external-facing endpoints.

---

## **10. Development Phases**

### **Phase 1: Core Cryptographic Functions**
- Implement encryption, decryption, obfuscation, and de-obfuscation.
- Build and test the CryptoEngine class.

### **Phase 2: REST API Integration**
- Expose cryptographic operations through a REST API.

### **Phase 3: Logging and Key Management**
- Integrate logging for auditability.
- Add basic key management capabilities.

### **Phase 4: Deployment and Optimization**
- Dockerize the service.
- Optimize performance for large-scale encryption/decryption tasks.

---

This design provides a robust and efficient plan for developing VitalEdge Crypt, ensuring high-performance cryptography and seamless integration with the ecosystem.