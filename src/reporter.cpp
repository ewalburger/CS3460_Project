#include "reporter.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include "packet_info.hpp"


std::string Reporter::protocolToString(TransportProtocol protocol) {
    switch (protocol) {
        case TransportProtocol::TCP: return "TCP";
        case TransportProtocol::UDP: return "UDP";
    }
    return "UNKNOWN";
}

FlowSnapshot Reporter::sortByPackets(const FlowSnapshot snapshot) const {
    FlowSnapshot sorted = snapshot;
    std::sort(sorted.begin(), sorted.end(),
        [](const FlowRow& a, const FlowRow& b) {
            return a.stats.packet_count > b.stats.packet_count;
        });
    return sorted;
}

FlowSnapshot Reporter::sortByBytes(const FlowSnapshot snapshot) const {
    FlowSnapshot sorted = snapshot;
    std::sort(sorted.begin(), sorted.end(),
        [](const FlowRow& a, const FlowRow& b) {
            return a.stats.byte_count > b.stats.byte_count;
        });
    return sorted;
}

std::string Reporter::summarize(const FlowSnapshot snapshot, const int interval, const int showing, std::string sortBy) const {
    std::string summary = "Interval: " + std::to_string(interval) + " s | Sort: " + sortBy + " Flows Stored: " + std::to_string(snapshot.size()) + " | Showing: " + std::to_string(showing) + "\n PROTO  SOURCE               DESTINATION           PACKETS    BYTES \n";
    
    if (!snapshot.empty()) {

    
        FlowSnapshot sorted = snapshot;

        if (sortBy == "packets") {
            sorted = Reporter::sortByPackets(snapshot);
        } else if (sortBy == "bytes") {
            sorted = sortByBytes(snapshot);
        } else {
            std::cerr << "Invalid sort option. Defaulting to sorting by packets." << std::endl;
            sorted = sortByPackets(snapshot);
        }

    
        for (size_t i = 0; i < showing && i < sorted.size(); ++i) {
            summary.append(sorted.at(i).key.protocol == TransportProtocol::TCP ? " TCP  " : " UDP  ");
            summary.append(sorted.at(i).key.source_ip + ":" + std::to_string(sorted.at(i).key.source_port));
            summary.append(" -> " + sorted.at(i).key.destination_ip + ":" + std::to_string(sorted.at(i).key.destination_port));
            summary.append("    " + std::to_string(sorted.at(i).stats.packet_count) + "    " + std::to_string(sorted.at(i).stats.byte_count));
            summary.append("\n");
        }

    }
    else{
        summary.append("No flows captured.");
    }
    return summary;
}




void Reporter::report(const FlowSnapshot snapshot, const int interval, const int showing, std::string sortBy) const {
     std::cout << summarize(snapshot, interval, showing, sortBy) << std::endl;
}