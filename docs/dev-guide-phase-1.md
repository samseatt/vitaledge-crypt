### **VitalEdge Crypt Phase 1 Developer's Guide**

This guide captures all the steps taken to develop and set up the **VitalEdge Crypt** project. It is designed to help developers start the project from scratch, understand the setup, and continue development smoothly without encountering previous issues.

---

## **1. Project Overview**
**VitalEdge Crypt** is a C++ microservice designed for encryption, decryption, and data obfuscation. It exposes a REST API for cryptographic operations and runs as a Dockerized container.

---

## **2. Prerequisites**

### **2.1. Operating System**
- macOS Ventura (Intel-based, compatible with `bash` shell).

### **2.2. Tools & Dependencies**
1. **Development Environment**
   - [VS Code](https://code.visualstudio.com/): IDE for coding.
   - CMake: Build system generator.
   - Docker Desktop: Containerization platform.

2. **Languages, Libraries, and Frameworks**
   - **C++17**: Core programming language.
   - **OpenSSL**: Cryptography library.
   - **jsoncpp**: JSON parsing library.
   - **Drogon**: REST API framework.
   - **Google Test (GTest)**: Unit testing framework.

3. **Package Managers**
   - **Homebrew**: For managing tools and dependencies.

4. **Bash Commands**
   Use `bash` as the terminal shell for commands.

---

## **3. Step-by-Step Setup**

### **3.1. Install Dependencies**

#### **Install Tools**
1. Install Homebrew (if not already installed):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

2. Install required tools:
   ```bash
   brew install cmake openssl jsoncpp googletest
   ```

3. Install Docker Desktop:
   - Download from [Docker's website](https://www.docker.com/products/docker-desktop).
   - Follow installation instructions and ensure it’s running.

4. Install Drogon Framework:
   ```bash
   brew install drogon
   ```

#### **Verify Installation**
- **CMake**:
  ```bash
  cmake --version
  ```
- **Docker**:
  ```bash
  docker --version
  docker info
  ```

---

### **3.2. Clone the Project**

1. Clone the repository:
   ```bash
   git clone https://github.com/<your-repo>/vitaledge-crypt.git
   cd vitaledge-crypt
   ```

2. Create necessary directories:
   ```bash
   mkdir build
   ```

---

### **3.3. Configure and Build the Project**

#### **Setup CMake**
1. Run `CMake` to generate build files:
   ```bash
   cmake -B build
   ```

2. Build the project:
   ```bash
   cmake --build build
   ```

#### **Verify the Build**
1. Run the executable:
   ```bash
   ./build/vitaledge-crypt
   ```

2. You should see:
   ```
   VitalEdge Crypt Microservice
   ```

---

### **3.4. Testing**

#### **Run Unit Tests**
1. Build and run tests:
   ```bash
   ./build/tests
   ```

2. Ensure all tests pass:
   ```
   [==========] Running 3 tests from 1 test case.
   [==========] All tests passed.
   ```

#### **API Testing**
Use `curl` to test REST API endpoints. Examples:

- **Encrypt Endpoint**:
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

### **3.5. Docker Containerization**

#### **Create Dockerfile**
1. Add the following `Dockerfile` in the project root:

   ```dockerfile
   # Use a lightweight base image
   FROM debian:bullseye-slim AS build

   WORKDIR /app

   # Install dependencies
   RUN apt-get update && apt-get install -y --no-install-recommends \
       build-essential cmake libssl-dev libjsoncpp-dev \
       && rm -rf /var/lib/apt/lists/*

   # Copy source code
   COPY . .

   # Build the application
   RUN cmake -B build && cmake --build build

   # Runtime stage
   FROM debian:bullseye-slim

   WORKDIR /app

   RUN apt-get update && apt-get install -y --no-install-recommends \
       libssl1.1 libjsoncpp1 \
       && rm -rf /var/lib/apt/lists/*

   # Copy the built executable
   COPY --from=build /app/build/vitaledge-crypt /app/vitaledge-crypt

   EXPOSE 8084

   CMD ["./vitaledge-crypt"]
   ```

#### **Build Docker Image**
1. Build the Docker image:
   ```bash
   docker build -t vitaledge-crypt .
   ```

2. Run the container:
   ```bash
   docker run -d -p 8084:8084 --name vitaledge-crypt vitaledge-crypt
   ```

---

## **4. Libraries, Classes, and Tools Used**

### **4.1. Libraries**
1. **OpenSSL**:
   - Provides encryption and decryption functionality.
   - Used for AES and RSA.

2. **jsoncpp**:
   - Parses and handles JSON for the REST API.

3. **Drogon**:
   - Exposes the REST API with routes (`/encrypt`, `/decrypt`, etc.).

4. **Google Test (GTest)**:
   - Ensures unit testing for cryptographic functions.

---

### **4.2. Classes**
1. **VitalEdgeCrypto**:
   - Implements encryption, decryption, and obfuscation.
   - AES and RSA cryptographic operations.

2. **Rest API Handlers**:
   - Implement endpoints using Drogon.

---

## **5. Remaining Tasks for Phase 2**

1. **Shared Libraries for Other Platforms**
   - **Kit-iOS** and **Kit-watchOS**:
     - Generate a shared library for Swift integration.
   - **Pi IoT**:
     - Compile a Python-compatible library using `pybind11`.

2. **Advanced Features**
   - Add key management support (e.g., AWS KMS).
   - Optimize performance for large datasets.

3. **Deployment**
   - Deploy to cloud platforms (AWS ECS or Kubernetes).

---

This guide captures the project setup and development journey.