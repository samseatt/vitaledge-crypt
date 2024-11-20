### **README.md for VitalEdge Crypt**

This README provides detailed instructions and information for developers working on the project.

# VitalEdge Crypt

VitalEdge Crypt is a high-performance, secure, and scalable cryptographic microservice built in C++. It provides encryption, decryption, and data obfuscation functionality via a REST API. The project is designed to serve as the central cryptographic engine for the VitalEdge ecosystem, with support for integration across iOS, watchOS, and IoT platforms.

---

## **Features**

- **Symmetric Encryption**: AES-256-CBC for secure data encryption.
- **Asymmetric Encryption**: RSA for key-based cryptographic operations.
- **Obfuscation**: Lightweight, reversible data masking.
- **Batch Operations**: Support for batch encryption and decryption.
- **Platform-Specific Libraries**:
  - iOS (Swift)
  - watchOS (Swift)
  - Raspberry Pi IoT (Python)
- **Centralized Key Management**:
  - Secure key storage and retrieval using services like AWS KMS or HashiCorp Vault.
- **REST API**:
  - Endpoints for cryptographic operations.

---

## **Getting Started**

### **1. Prerequisites**

- macOS Ventura (or equivalent Linux environment)
- CMake (Build system)
- Docker Desktop (for containerization)
- Homebrew (for package management)

### **2. Installation**

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/<your-repo>/vitaledge-crypt.git
   cd vitaledge-crypt
   ```

2. **Install Dependencies**:
   ```bash
   brew install cmake openssl jsoncpp drogon googletest
   ```

3. **Build the Project**:
   ```bash
   cmake -B build
   cmake --build build
   ```

4. **Run the Application**:
   ```bash
   ./build/vitaledge-crypt
   ```

### **3. REST API**

Run the application and access the REST API at `http://localhost:8084`.

#### **Endpoints**:
- **Encrypt**:
  - `POST /encrypt`
  - Input: JSON with `data`, `key`, and `iv`.
  - Output: Base64-encoded ciphertext.

- **Decrypt**:
  - `POST /decrypt`
  - Input: JSON with `data`, `key`, and `iv`.
  - Output: Base64-encoded plaintext.

- **Obfuscate**:
  - `POST /obfuscate`
  - Input: JSON with `data`.
  - Output: Base64-encoded obfuscated data.

- **Deobfuscate**:
  - `POST /deobfuscate`
  - Input: JSON with `data`.
  - Output: Original plaintext.

Example `curl` command for encryption:
```bash
curl -X POST http://localhost:8084/encrypt \
-H "Content-Type: application/json" \
-d '{
    "data": "Hello, VitalEdge!",
    "key": "0123456789abcdef0123456789abcdef",
    "iv": "abcdef0123456789"
}'
```

---

## **Building Docker Image**

1. **Build the Docker Image**:
   ```bash
   docker build -t vitaledge-crypt .
   ```

2. **Run the Container**:
   ```bash
   docker run -d -p 8084:8084 --name vitaledge-crypt vitaledge-crypt
   ```

3. **Test the API**:
   Use `curl` or Postman to test endpoints running inside the container.

---

## **Platform-Specific Libraries**

The project supports integration with external platforms:

### **1. iOS (Kit-iOS)**:
- Shared library for Swift integration.
- Build using CMake and export as `.framework` for Xcode.

### **2. watchOS (Kit-watchOS)**:
- Lightweight shared library for obfuscation functionality.
- Optimized for Apple Watch hardware constraints.

### **3. Pi IoT (Python)**:
- Python-compatible library using `pybind11`.
- Expose C++ functions directly to Python.

---

## **Testing**

1. **Unit Tests**:
   ```bash
   ./build/tests
   ```

2. **Example Output**:
   ```
   [==========] Running 3 tests from 1 test case.
   [==========] All tests passed.
   ```

---

## **Future Enhancements**

1. **Key Management**:
   - Integrate with AWS KMS or HashiCorp Vault.
   - Add key rotation support.

2. **Advanced Cryptographic Features**:
   - Support elliptic curve cryptography (ECC).
   - Add tokenization for sensitive data masking.

3. **Deployment**:
   - Deploy container to AWS ECS or Kubernetes.

---

## **Contributing**

We welcome contributions! Follow these steps to get involved:

1. Fork the repository.
2. Create a feature branch:
   ```bash
   git checkout -b feature-name
   ```
3. Commit your changes:
   ```bash
   git commit -m "Add feature-name"
   ```
4. Push to your branch:
   ```bash
   git push origin feature-name
   ```
5. Submit a Pull Request.

---

## **License**

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## **Acknowledgments**

- **OpenSSL**: Cryptographic library for encryption and decryption.
- **jsoncpp**: JSON parsing library.
- **Drogon**: High-performance C++ web framework.
- **Google Test**: Unit testing framework.

---

## **Contact**

For questions or feedback, contact the repository owner at:
- **Email**: your-email@example.com
- **GitHub**: [Your GitHub Profile](https://github.com/<your-profile>)
