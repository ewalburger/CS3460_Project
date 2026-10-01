#include "reporter.hpp"
#include <iostream>
#include <sstream>

std::string Reporter::protocolToString(TransportProtocol protocol) {
    switch (protocol) {
        case TransportProtocol::TCP: return "TCP";
        case TransportProtocol::UDP: return "UDP";
    }
    return "UNKNOWN";
}

std::string Reporter::summarize(const PacketInfo& packet) const {
    std::ostringstream oss;
    oss << protocolToString(packet.protocol) << " "
        << packet.source_ip << ":" << packet.source_port
        << " -> "
        << packet.destination_ip << ":" << packet.destination_port
        << " (" << packet.wire_bytes << " bytes)";
    return oss.str();
}

void Reporter::report(const PacketInfo& packet) const {
    std::cout << summarize(packet) << std::endl;
}