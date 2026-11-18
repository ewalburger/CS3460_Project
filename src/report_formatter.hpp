#pragma once

#include "flow_aggregator.hpp"
#include "report_configuration.hpp"
#include "sort_strategy.hpp"

class ReportFormatter {
public:
    void report(const FlowSnapshot& snapshot,
                const ReportConfiguration& configuration,
                const SortStrategy& strategy) const;
    std::string format(const FlowSnapshot& snapshot,
                       const ReportConfiguration& configuration,
                       const SortStrategy& strategy) const;
};