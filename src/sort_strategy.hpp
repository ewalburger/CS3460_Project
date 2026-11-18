#pragma once

#include "flow_aggregator.hpp"

class SortStrategy {
public:
    virtual ~SortStrategy() = default;
    virtual FlowSnapshot sort(const FlowSnapshot& snapshot) const = 0;
};

class PacketCountSortStrategy final : public SortStrategy {
public:
    FlowSnapshot sort(const FlowSnapshot& snapshot) const override;
};

class ByteCountSortStrategy final : public SortStrategy {
public:
    FlowSnapshot sort(const FlowSnapshot& snapshot) const override;
};
