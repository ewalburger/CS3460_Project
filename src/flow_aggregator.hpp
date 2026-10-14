struct FlowStats {
    std::uint64_t packet_count;
    std::uint64_t byte_count;
};

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