# shared/CMakeLists.txt

# Add the VitalEdgeCrypto shared library
add_library(VitalEdgeCrypto SHARED
    VitalEdgeCrypto.cpp
    VitalEdgeKeyManager.cpp
    VitalEdgeUtils.cpp
)

# Specify include directories for the shared library
target_include_directories(VitalEdgeCrypto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link OpenSSL to the shared library (if it depends on OpenSSL)
find_package(OpenSSL REQUIRED PATHS /usr/local/opt/openssl@3 NO_DEFAULT_PATH)
target_link_libraries(VitalEdgeCrypto PUBLIC OpenSSL::SSL OpenSSL::Crypto)
