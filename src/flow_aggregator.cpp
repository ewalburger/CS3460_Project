#include "flow_aggregator.hpp"

void FlowAggregator::update(const Packet& packet) {
    std::lock_guard<std::mutex> lock(mutex_);
    FlowKey key{packet.protocol, packet.source_ip, packet.source_port,
                packet.destination_ip, packet.destination_port};
    
    //check if the FlowKey already exists in the map
    auto key_iterator = flows_.find(key);
    if (key_iterator == flows_.end()) {
        key_iterator = flows_.emplace(key, FlowRow{key}).first;
    }
    key_iterator->second.recordPacket(packet.wire_bytes);
}

FlowSnapshot FlowAggregator::snapshot() const {
    std::lock_guard<std::mutex> lock(mutex_);
    FlowSnapshot rows;
    rows.reserve(flows_.size());
    for (const std::pair<const FlowKey, FlowRow>& entry : flows_) {
        rows.push_back(entry.second);
    }
    return rows;
}
