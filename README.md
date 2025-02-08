# Distributed Key-Value Store

DistributedKeyValueStore is a distributed key–value storage system implemented in C++ using gRPC. The project uses consistent hashing for data distribution across nodes and includes heartbeat-based failure detection and a join protocol so that nodes can dynamically update the cluster membership.

## Overview

This project demonstrates a simple distributed key–value store with the following core features:
- **Consistent Hashing:**  
  Keys are mapped to nodes based on a hash ring, ensuring even distribution and minimizing remapping when nodes join or leave.
- **gRPC-based Communication:**  
  Nodes communicate using gRPC for operations such as `Put`, `Get`, `Heartbeat`.
- **Heartbeat Manager:**  
  Periodic heartbeat messages are sent between nodes to detect failures. When a node failure is detected, the failed node is removed from the hash ring.
- **Command-line Interactive Interface:**  
  Each node can accept commands (like `put` and `get`) via an interactive command loop.
- Current architecture does not support replication, if one node fails then no way to restore those keys from that node.
- Current architecture does not support if node fails and restarts again then other nodes won't know that new node is added.

## Demo
![demo](https://github.com/rahul07bagul/DistributedKeyValueStore/blob/main/demo.png)

## Design
![Design](https://github.com/rahul07bagul/DistributedKeyValueStore/blob/main/design.png)

## Prerequisites

- **C++ Compiler:**  
  A modern C++ compiler that supports C++11 or later.
- **gRPC & Protocol Buffers:**  
  Install gRPC and Protocol Buffers. For Windows, you can use vcpkg:
  ```bash
  vcpkg install grpc protobuf
  ```
- CMake: To configure and build the project.

## Installation and Build
- Download vcpkg
```shell
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

- Install gRPC and protobuf
```shell
.\vcpkg install grpc:x64-windows
.\vcpkg install protobuf:x64-windows
.\vcpkg integrate install
```

- Compile proto file
```shell
& "<your_path>\DistributedKeyValueStore\vcpkg\installed\x64-windows\tools\protobuf\protoc.exe" -I=network --cpp_out=network network/rpc_service.proto
& "<your_path>\DistributedKeyValueStore\vcpkg\installed\x64-windows\tools\protobuf\protoc.exe" `
    -I=network `
    --grpc_out=network `
    --plugin=protoc-gen-grpc="<your_path>\vcpkg\installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe" `
    network\rpc_service.proto 
```

- Using powershell7: it will create .exe file in build/Release folder
```shell
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
OR
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="<your_path>/DistributedKeyValueStore/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build build --config Release
```

- Run the code from root, each command in separate terminal (three servers)
```shell
.\build\Release\distributed_kv_store.exe --node-id=node1 --address=127.0.0.1:50051 --peers=node2:127.0.0.1:50052,node3:127.0.0.1:50053
.\build\Release\distributed_kv_store.exe --node-id=node2 --address=127.0.0.1:50052 --peers=node1:127.0.0.1:50051,node3:127.0.0.1:50053
.\build\Release\distributed_kv_store.exe --node-id=node3 --address=127.0.0.1:50053 --peers=node1:127.0.0.1:50051,node2:127.0.0.1:50052
```

- Test
  - From Node1:
    ```shell
     put foo bar
    ```
  - From Node2:
    ```shell
    get foo
    ```
  
