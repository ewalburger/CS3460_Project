#pragma once

#include "packet_info.hpp"
#include "flow_aggregator.hpp"

class Reporter {

private:
    FlowSnapshot sortByPackets(const FlowSnapshot snapshot) const;
    FlowSnapshot sortByBytes(const FlowSnapshot snapshot) const;

    
public:

    void report(const PacketInfo& packet) const;

    static std::string protocolToString(TransportProtocol protocol);

    std::string summarize(const FlowSnapshot snapshot, const int interval, const int showing, std::string sortBy) const;
};