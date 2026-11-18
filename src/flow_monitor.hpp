#pragma once

#include <string>

#include "flow_aggregator.hpp"
#include "packet_queue.hpp"
#include "pcap_capture.hpp"
#include "report_configuration.hpp"
#include "report_formatter.hpp"

class FlowMonitor {
public:
    FlowMonitor(std::string interface_name, ReportConfiguration configuration);

    void run();

private:
    void reportFlows() const;

    std::string interface_name_;
    ReportConfiguration configuration_;
    PcapCapture capture_;
    PacketQueue packet_queue_;
    FlowAggregator aggregator_;
    ReportFormatter formatter_;
};
