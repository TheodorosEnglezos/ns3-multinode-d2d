#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/csma-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("MultiD2DSimulation");

double CalculateEuclideanDistance(const Vector &position1, const Vector &position2)
{
    double dx = position1.x - position2.x;
    double dy = position1.y - position2.y;
    double dz = position1.z - position2.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

void ProximityCheck(NodeContainer nodes, double threshold, std::vector<Ptr<Socket>> sockets, Ipv4InterfaceContainer interfaces)
{
    for (uint32_t i = 0; i < nodes.GetN(); ++i)
    {
        Ptr<MobilityModel> mobility1 = nodes.Get(i)->GetObject<MobilityModel>();
        Vector position1 = mobility1->GetPosition();

        for (uint32_t j = i + 1; j < nodes.GetN(); ++j)
        {
            Ptr<MobilityModel> mobility2 = nodes.Get(j)->GetObject<MobilityModel>();
            Vector position2 = mobility2->GetPosition();

            double distance = CalculateEuclideanDistance(position1, position2);
            NS_LOG_INFO("Proximity check between node " << nodes.Get(i)->GetId() << " and node " << nodes.Get(j)->GetId());
            NS_LOG_INFO("Distance between nodes: " << distance << " meters");

            if (distance <= threshold)
            {
                NS_LOG_INFO("Nodes are within proximity. Establishing D2D communication.");
                Ptr<Packet> packet = Create<Packet>(1024);
                sockets[i]->SendTo(packet, 0, InetSocketAddress(interfaces.GetAddress(j), 9));
            }
            else
            {
                NS_LOG_INFO("Nodes are NOT within proximity. No D2D communication.");
            }
        }
    }
}

void ReceivePacket(Ptr<Socket> socket)
{
    while (socket->Recv())
    {
        NS_LOG_INFO("Packet received at node " << socket->GetNode()->GetId());
    }
}

int main(int argc, char *argv[])
{
    LogComponentEnable("MultiD2DSimulation", LOG_LEVEL_INFO);

    uint32_t numNodes = 5; // Αριθμός κόμβων
    double threshold = 10.0; // 10 μέτρα όριο για D2D επικοινωνία

    NodeContainer nodes;
    nodes.Create(numNodes);

    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue(0.0),
                                  "MinY", DoubleValue(0.0),
                                  "DeltaX", DoubleValue(5.0),
                                  "DeltaY", DoubleValue(10.0),
                                  "GridWidth", UintegerValue(3),
                                  "LayoutType", StringValue("RowFirst"));

    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(nodes);

    InternetStackHelper internet;
    internet.Install(nodes);

    CsmaHelper csma;
    NetDeviceContainer devices = csma.Install(nodes);

    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign(devices);

    TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
    std::vector<Ptr<Socket>> sockets;

    for (uint32_t i = 0; i < numNodes; ++i)
    {
        Ptr<Socket> socket = Socket::CreateSocket(nodes.Get(i), tid);
        sockets.push_back(socket);
        InetSocketAddress local = InetSocketAddress(Ipv4Address::GetAny(), 9);
        socket->Bind(local);
        socket->SetRecvCallback(MakeCallback(&ReceivePacket));
    }

    Simulator::Schedule(Seconds(1.0), &ProximityCheck, nodes, threshold, sockets, interfaces);

    AnimationInterface anim("multi_d2d.xml");
    anim.EnablePacketMetadata(true);
    anim.SetMaxPktsPerTraceFile(1000);
    for (uint32_t i = 0; i < numNodes; ++i)
    {
        anim.UpdateNodeDescription(nodes.Get(i), "Node " + std::to_string(i));
    }

    Simulator::Stop(Seconds(5.0));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}

