# ns3-multinode-d2d (Mini Project)
# 0. Overview
The simulation creates multiple wireless nodes that communicate directly with each other based on their physical proximity. When two nodes are within a predefined distance threshold, they establish a D2D connection and exchange UDP packets.

The simulation also supports visualization using NetAnim, allowing users to observe packet transmissions between nodes in real time.

# 1. Run the simulation
./ns3 run scratch/Multid2d.cc

# 2. Launch NetAnim to visualize the simulation animation
./NetAnim
