# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.15)

# Set the project name
project(VitalEdgeCrypt VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Locate OpenSSL
find_package(OpenSSL REQUIRED PATHS /usr/local/opt/openssl@3 NO_DEFAULT_PATH)

# Verify OpenSSL paths (useful for debugging)
message(STATUS "OpenSSL Include Directory: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL Libraries: ${OPENSSL_LIBRARIES}")

# Add the shared library directory
add_subdirectory(shared)

# Define an INTERFACE library for common dependencies
add_library(common_libs INTERFACE)

# Add common include directories and link libraries to the INTERFACE library
target_include_directories(common_libs INTERFACE
    ${CMAKE_SOURCE_DIR}/shared
    ${OPENSSL_INCLUDE_DIR}
)
target_link_libraries(common_libs INTERFACE
    VitalEdgeCrypto
    OpenSSL::SSL OpenSSL::Crypto
)

# Add the main executable
add_executable(vitaledge-crypt src/main.cpp)

# Link the main executable to the common libraries
target_link_libraries(vitaledge-crypt PRIVATE common_libs)

# Enable testing
enable_testing()

# Find Google Test
find_package(GTest REQUIRED)

# Add the test executable
add_executable(tests tests/test_vitaledge_crypto.cpp)

# Link the test executable to the common libraries and GTest
target_link_libraries(tests PRIVATE common_libs GTest::GTest GTest::Main)

# Find Drogon
find_package(Drogon REQUIRED)

# Include Drogon's directories
include_directories(${Drogon_INCLUDE_DIRS})

# Link Drogon and its dependencies (e.g., Trantor) to the main executable
target_link_libraries(vitaledge-crypt PRIVATE Drogon::Drogon OpenSSL::SSL OpenSSL::Crypto)
