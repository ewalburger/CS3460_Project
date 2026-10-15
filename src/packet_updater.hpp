#pragma once

#include <string>
#include <cstdint>
#include "packet_parser.hpp"

struct FlowKey {
    TransportProtocol protocol{}; 
    std::string source_ip;
    std::uint16_t source_port{};
    std::string destination_ip;
    std::uint16_t destination_port{};
};

struct FlowStats {
    std::uint64_t packets{};
    std::uint64_t bytes{};
};

// Place operator< right below the struct in the header
inline bool operator<(const FlowKey& a, const FlowKey& b) {
    if (a.protocol != b.protocol) return a.protocol < b.protocol;
    if (a.source_ip != b.source_ip) return a.source_ip < b.source_ip;
    if (a.source_port != b.source_port) return a.source_port < b.source_port;
    if (a.destination_ip != b.destination_ip) return a.destination_ip < b.destination_ip;
    return a.destination_port < b.destination_port;
}