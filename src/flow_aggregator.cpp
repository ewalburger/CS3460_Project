// bool operator<(const FlowKey& a, const FlowKey& b) {
// if (a.protocol != b.protocol) return a.protocol < b.protocol;
// if (a.source_ip != b.source_ip) return a.source_ip < b.source_ip;
// if (a.source_port != b.source_port)
// return a.source_port < b.source_port;
// if (a.destination_ip != b.destination_ip)
// return a.destination_ip < b.destination_ip;
// return a.destination_port < b.destination_port;
// }




void FlowAggregator::update(const PacketInfo& packet) {
    FlowKey key{packet.protocol, packet.source_ip, packet.source_port,
                packet.destination_ip, packet.destination_port};

    auto it = flows_.find(key);
    if (it != flows_.end()) {
        it->second.packet_count += 1;
        it->second.byte_count += packet.size;
    } else {
        flows_[key] = FlowStats{1, packet.size};
    }
}

FlowSnapshot FlowAggregator::snapshot() const {
    FlowSnapshot snapshot;
    for (const auto& entry : flows_) {
        snapshot.push_back(FlowRow{entry.first, entry.second});
    }
    return snapshot;
}
