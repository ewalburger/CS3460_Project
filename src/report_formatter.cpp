#include "report_formatter.hpp"
#include <algorithm>
#include <iostream>

FlowSnapshot ReportFormatter::sortByPackets(const FlowSnapshot snapshot) const {
    FlowSnapshot sorted = snapshot;
    std::sort(sorted.begin(), sorted.end(),
        [](const FlowRow& a, const FlowRow& b) {
            return a.stats.packet_count > b.stats.packet_count;
        });
    return sorted;
}

FlowSnapshot ReportFormatter::sortByBytes(const FlowSnapshot snapshot) const {
    FlowSnapshot sorted = snapshot;
    std::sort(sorted.begin(), sorted.end(),
        [](const FlowRow& a, const FlowRow& b) {
            return a.stats.byte_count > b.stats.byte_count;
        });
    return sorted;
}

std::string ReportFormatter::format(
    const FlowSnapshot& snapshot, const ReportConfiguration& configuration) const {
    const std::string sort_name =
        configuration.sortCriterion() == SortCriterion::PACKETS ? "packets" : "bytes";
    std::string summary = "Interval: " + std::to_string(configuration.interval()) +
        " s | Sort: " + sort_name + " Flows Stored: " +
        std::to_string(snapshot.size()) + " | Showing: " +
        std::to_string(configuration.numberToShow()) +
        "\n PROTO  SOURCE               DESTINATION           PACKETS    BYTES \n";
    
    if (!snapshot.empty()) {

    
        FlowSnapshot sorted = snapshot;

        if (configuration.sortCriterion() == SortCriterion::PACKETS) {
            sorted = sortByPackets(snapshot);
        } else {
            sorted = sortByBytes(snapshot);
        }

    
        for (size_t i = 0; i < static_cast<size_t>(configuration.numberToShow()) &&
                       i < sorted.size(); ++i) {
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




void ReportFormatter::report(
    const FlowSnapshot& snapshot, const ReportConfiguration& configuration) const {
     std::cout << format(snapshot, configuration) << std::endl;
}