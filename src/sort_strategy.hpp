#pragma once

#include "flow_aggregator.hpp"

class SortStrategy {
public:
    SortStrategy() = default;
    virtual ~SortStrategy() = default;
    virtual FlowSnapshot sort(const FlowSnapshot& snapshot) const = 0;
};

class PacketCountSortStrategy final : public SortStrategy {
public:
    PacketCountSortStrategy() = default;
    ~PacketCountSortStrategy() override = default;

    FlowSnapshot sort(const FlowSnapshot& snapshot) const override;
};

class ByteCountSortStrategy final : public SortStrategy {
public:
    ByteCountSortStrategy() = default;
    ~ByteCountSortStrategy() override = default;

    FlowSnapshot sort(const FlowSnapshot& snapshot) const override;
};
