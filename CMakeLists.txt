cmake_minimum_required(VERSION 3.13)
project(DistributedKeyValueStore LANGUAGES CXX)

# -----------------------------------------------------------------------------
# 1) Vcpkg Integration
# -----------------------------------------------------------------------------
# Ensure CMake knows to use vcpkg's toolchain file for library discovery.
# If you run:
#    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="R:/C++/Projects/DistributedKeyValueStore/vcpkg/scripts/buildsystems/vcpkg.cmake"
# you can omit this 'set' call. Alternatively, keep it here so you don't need
# to pass the toolchain file each time.
#
set(CMAKE_TOOLCHAIN_FILE
    "R:/C++/Projects/DistributedKeyValueStore/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE FILEPATH "Vcpkg toolchain file" FORCE
)

# -----------------------------------------------------------------------------
# 2) C++ Standard and MSVC Warnings
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# If using MSVC, suppress the C4251 dll-interface warnings that Protobuf often triggers.
if(MSVC)
    add_compile_options("/wd4251")
endif()

# -----------------------------------------------------------------------------
# 3) External Dependencies via vcpkg
# -----------------------------------------------------------------------------
# vcpkg will provide gRPC, Protobuf, OpenSSL, and ZLIB.
# The OPENSSL_ROOT_DIR and ZLIB_ROOT lines are optional if find_package()
# can auto-locate them via the vcpkg toolchain. If needed, keep them:
#
set(OPENSSL_ROOT_DIR "R:/C++/Projects/DistributedKeyValueStore/vcpkg/installed/x64-windows")
set(ZLIB_ROOT        "R:/C++/Projects/DistributedKeyValueStore/vcpkg/installed/x64-windows")

find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

# -----------------------------------------------------------------------------
# 4) Executable
# -----------------------------------------------------------------------------
add_executable(distributed_kv_store
    src/main.cpp
    src/core/node.cpp
    src/core/consistent_hash.cpp
    src/core/heartbeat_manager.cpp
    src/core/command_handler.cpp
    src/core/node_initializer.cpp
    src/core/input_parser.cpp
    src/network/network_manager.cpp
    src/storage/key_value_store.cpp
    src/network/rpc_service.pb.cc
    src/network/rpc_service.grpc.pb.cc
)

# -----------------------------------------------------------------------------
# 5) Include Paths
# -----------------------------------------------------------------------------
# vcpkg includes are automatically added, but if you need to reference your own
# headers or the generated .pb.h files, do so here:
target_include_directories(distributed_kv_store PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${OPENSSL_ROOT_DIR}/include"
    "${ZLIB_ROOT}/include"
)

# -----------------------------------------------------------------------------
# 6) Link Libraries
# -----------------------------------------------------------------------------
# Link against the libraries discovered via find_package and vcpkg.
target_link_libraries(distributed_kv_store PRIVATE
    gRPC::grpc++              # gRPC library
    protobuf::libprotobuf     # Protobuf
    OpenSSL::SSL             # OpenSSL SSL
    OpenSSL::Crypto          # OpenSSL Crypto
    ZLIB::ZLIB               # ZLIB
)

# -----------------------------------------------------------------------------
# Print the build type (useful for debugging).
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
