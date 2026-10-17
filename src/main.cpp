#include <iostream>
#include <pcap/pcap.h>
#include "pcap_capture.hpp"
#include "reporter.hpp"
#include "flow_aggregator.hpp"


 
int main()
{
    try {
        auto interfaces = get_and_display_interfaces();

        if (interfaces.empty()) {
            std::cerr << "No network interfaces found." << std::endl;
            return 1;
        }
        // user needs to choose an interface
        size_t choice = 0;
        std::cout << "\nEnter the interface number you want to use: ";
        std::cin >> choice;

        // call open_pcap_handle with the chosen interface
        if (choice < 1 || choice > interfaces.size()) {
            std::cerr << "Invalid choice." << std::endl;
            return 1;
        }
        auto pcap_handle = open_pcap_handle(interfaces[choice - 1]);
        const auto packets = capture_loop(pcap_handle, 10); // Capture 10 packets

        std::cout << "Opened interface: " << interfaces[choice - 1] << std::endl;
        Reporter reporter;
        FlowAggregator aggregator;
        for (const PacketInfo& packet : packets) {
            std::cout << "hi, this is printing" << std::endl;
            aggregator.update(packet);
            //reporter.report(packet);
        }

        const auto snapshot = aggregator.snapshot();
        std::cout << "\nFlow Summary:\n";
        for (const FlowRow& row : snapshot) {
            std::cout << row.key.source_ip << ":" << row.key.source_port
                      << " -> "
                      << row.key.destination_ip << ":" << row.key.destination_port
                      << " Protocol: " << Reporter::protocolToString(row.key.protocol)
                      << " Packets: " << row.stats.packet_count
                      << " Bytes: " << row.stats.byte_count
                      << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
