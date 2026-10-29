# Comm Sim - C++ Communications Network Simulator:

A C++ Communications Network Simulator designed to model communications between network nodes under configurable network conditions. 
The goal is to build a modular simulator that demonstrates C++ programming, network communication concepts and systems-level software development.

## Features
- Node management: Create and manage nodes independently from the network. When nodes are registered to the network, they can be managed and tracked after creation.
- Message transmission: Send messages between nodes with configurable message types and priorities.
- Simulated network conditions: Configurable packet-loss probability and transmission latency to simulate real network conditions.
- Node failure and recovery: Simulate nodes going offline and return to service.
- Heartbeat monitoring: Exchange heartbeat and heartbeat acknowledgement messages
- Timeout detection: Track missed heartbeats and identify offline nodes

## Configuration
Simulation parameters and node setup are currently configured in src/main.cpp.

To experiment with different network conditions:

1. Open src/main.cpp.
2. Adjust the packet-loss probability and minimum/maximum latency settings.
3. Modify the node setup as needed.
4. Rebuild the project using the instructions below.

Changes to the source code are required to change these settings; command-line configuration is a planned improvement.

## Build and Run
### Prerequisites
- A C++ compiler that supports the C++ standard used by the project
- CMake
- Git

### Build 
Clone the repository and enter the project directory: \
`git clone https://github.com/badluckguy5/Comm-sim.git` \
`cd comm-sim`

Configure and Build: \
`cmake -S . -B build` \
`cmake --build build` 

Run the simulator: \
`./build/comm_sim` 

## Project Structure:
Comm-sim/ \
├── include/ &emsp; &emsp;&emsp; # Header files \
├── src/ &emsp; &emsp; &emsp; &emsp;&ensp; # C++ implementation files \
├── tests/ &emsp;  &emsp; &emsp; &emsp;# Tests and test code \
├── CMakeLists.txt &emsp;# CMake build configuration \
└── README.md

## Design Goals
Comm Sim is being developed incrementally, with an emphasis on modular design, clear separation of responsibilities, and realistic communication failure scenarios.

## Planned Improvments
- Continuous and concurrent heartbeat monitoring
- Multithreaded node monitoring and graceful shutdown
- UDP and TCP communication between separate simulator processes
- Packet serialization and deserialization
- Expanded automated testing and network failure scenarios

