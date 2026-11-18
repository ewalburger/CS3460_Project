#include "sort_strategy.hpp"

#include <algorithm>

FlowSnapshot PacketCountSortStrategy::sort(const FlowSnapshot& snapshot) const {
    FlowSnapshot sorted = snapshot;
    std::sort(sorted.begin(), sorted.end(),
              [](const FlowRow& first, const FlowRow& second) {
                  return first.stats.packet_count > second.stats.packet_count;
              });
    return sorted;
}

FlowSnapshot ByteCountSortStrategy::sort(const FlowSnapshot& snapshot) const {
    FlowSnapshot sorted = snapshot;
    std::sort(sorted.begin(), sorted.end(),
              [](const FlowRow& first, const FlowRow& second) {
                  return first.stats.byte_count > second.stats.byte_count;
              });
    return sorted;
}
