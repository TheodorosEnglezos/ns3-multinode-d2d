# ns3-multinode-d2d (Mini Project)
# 0. Overview
The simulation creates multiple wireless nodes that communicate directly with each other based on their physical proximity. When two nodes are within a predefined distance threshold, they establish a D2D connection and exchange UDP packets.

The simulation also supports visualization using NetAnim, allowing users to observe packet transmissions between nodes in real time.

# Features
- Multi-node D2D communication (5 nodes)
- Proximity-based connection establishment
- UDP socket-based packet transmission
- Real-time packet visualization with NetAnim
- Distance calculation using Euclidean model

# How It Works
- Nodes are placed in a grid layout
- Each node checks the distance to every other node
- If the distance is below a threshold:
  - A packet is sent using UDP sockets
- Packet transmissions are visualized in NetAnim

# Visualization
The simulation generates an XML file that can be opened in NetAnim to visualize:
- Node positions
- Packet transmissions between nodes
- Dynamic communication links

# Technologies Used
- ns-3
- C++
- NetAnim

# 1. Run the Simulation
./ns3 run scratch/Multid2d.cc

# 2. Launch NetAnim to visualize the simulation animation
./NetAnim

# Dont Forget before run the Simulation
Build ns-3
