#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include "packet_info.hpp"

struct FlowKey {
    TransportProtocol protocol{}; 
    std::string source_ip;
    std::uint16_t source_port{};
    std::string destination_ip;
    std::uint16_t destination_port{};
};

struct FlowStats {
    std::uint64_t packet_count;
    std::uint64_t byte_count;
};

// std::map<FlowKey, FlowStats>;

// Compare FlowKeys for ordering in std::map. This is a lexicographical comparison of the fields.
inline bool operator<(const FlowKey& a, const FlowKey& b) {
    if (a.protocol != b.protocol) return a.protocol < b.protocol;
    if (a.source_ip != b.source_ip) return a.source_ip < b.source_ip;
    if (a.source_port != b.source_port) return a.source_port < b.source_port;
    if (a.destination_ip != b.destination_ip) return a.destination_ip < b.destination_ip;
    return a.destination_port < b.destination_port;
}

struct FlowRow { 
    FlowKey key; 
    FlowStats stats; 
};
using FlowTable = std::map<FlowKey, FlowStats>;
using FlowSnapshot = std::vector<FlowRow>; // Reporting copy.


class FlowAggregator {
public:
    void update(const PacketInfo& packet);
    FlowSnapshot snapshot() const;
private:
    FlowTable flows_; // Persists across reports.
};