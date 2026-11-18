#pragma once

#include "flow_aggregator.hpp"
#include "report_configuration.hpp"

class ReportFormatter {

private:
    FlowSnapshot sortByPackets(const FlowSnapshot snapshot) const;
    FlowSnapshot sortByBytes(const FlowSnapshot snapshot) const;

public:
    void report(const FlowSnapshot& snapshot,
                const ReportConfiguration& configuration) const;
    std::string format(const FlowSnapshot& snapshot,
                       const ReportConfiguration& configuration) const;
};