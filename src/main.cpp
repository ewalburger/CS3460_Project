#include <iostream>
#include <pcap/pcap.h>
#include "pcap_capture.hpp"
#include "reporter.hpp"
#include "flow_aggregator.hpp"
#include <exception>
#include <thread>

 
int main()
{
    try {
        auto interfaces = get_and_display_interfaces();

        if (interfaces.empty()) {
            std::cerr << "No network interfaces found." << std::endl;
            return 1;
        }
        // user needs to choose an interface
        size_t interface_choice = 0;
        std::cout << "\nEnter the interface number you want to use: ";
        std::cin >> interface_choice;

        // call open_pcap_handle with the chosen interface
        if (interface_choice < 1 || interface_choice > interfaces.size()) {
            std::cerr << "Invalid choice." << std::endl;
            return 1;
        }

        size_t time_choice = 0;
        std::cout << "\n Enter time interval (1-2 seconds): ";
        std::cin >> time_choice;

        if (time_choice < 1 || time_choice > 2) {
            std::cerr << "Invalid time interval." << std::endl;
            return 1;
        }

        auto pcap_handle = open_pcap_handle(interfaces[interface_choice - 1]);

        std::cout << "Opened interface: " << interfaces[interface_choice - 1] << std::endl;
        FlowAggregator aggregator;
        PacketQueue packets;
        std::exception_ptr capture_error;

        std::thread capture_thread([&] {
            try {
                capture_loop(pcap_handle, 10, packets); // Capture 10 packets
            } catch (...) {
                capture_error = std::current_exception();
            }
            packets.close();
        });

        std::thread update_thread([&] {
            while (const auto packet = packets.pop()) {
                aggregator.update(*packet);
            }
        });

        capture_thread.join();
        update_thread.join();

        if (capture_error) {
            std::rethrow_exception(capture_error);
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
