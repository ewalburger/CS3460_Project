#include "flow_aggregator.hpp"

void FlowAggregator::update(const PacketInfo& packet) {
    FlowKey key{packet.protocol, packet.source_ip, packet.source_port,
                packet.destination_ip, packet.destination_port};
    
    //check if the FlowKey already exists in the map
    auto key_iterator = flows_.find(key);
    if (key_iterator != flows_.end()) {
        key_iterator->second.packet_count += 1;
        key_iterator->second.byte_count += packet.wire_bytes;
    } else {
        flows_[key] = FlowStats{1, packet.wire_bytes};
    }
}
// this one was generated 
// FlowSnapshot FlowAggregator::snapshot() const {
//     FlowSnapshot snapshot;
//     for (const auto& entry : flows_) {
//         snapshot.push_back(FlowRow{entry.first, entry.second});
//     }
//     return snapshot;
// }

FlowSnapshot FlowAggregator::snapshot() const {
    FlowSnapshot rows;
    rows.reserve(flows_.size());
    for (const std::pair<const FlowKey, FlowStats>& entry : flows_) {
        rows.push_back(FlowRow{entry.first, entry.second});
    }
    return rows;
}
