#include "packet_updater.hpp"

// function for making the key for the map
FlowKey make_flow_key(const PacketInfo& packet) {
    return FlowKey{
        packet.protocol,
        packet.source_ip,
        packet.source_port,
        packet.destination_ip,
        packet.destination_port
    };
}

// function for updating the flow stats
void update_flow_stats(const FlowKey& key, std::uint64_t packet_size) {
    auto& flow_stats = flow_table[key];
    flow_stats.packets++;
    flow_stats.bytes += packet_size;
}

//this part should go in the flow aggregator
// #include <map>

// std::map<FlowKey, FlowStats> flow_table;
