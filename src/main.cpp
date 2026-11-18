#include <iostream>
#include <string>

#include "flow_monitor.hpp"
#include "network_interface_manager.hpp"

int main() {
    try {
        NetworkInterfaceManager interface_manager;
        const auto interfaces = interface_manager.discoverInterfaces();

        if (interfaces.empty()) {
            std::cerr << "No network interfaces found." << std::endl;
            return 1;
        }
        interface_manager.displayInterfaces();

        std::size_t interface_choice = 0;
        std::cout << "\nEnter the interface number you want to use: ";
        std::cin >> interface_choice;

        std::cout << "\nEnter the interval in seconds for flow reporting: ";
        int interval = 0;
        std::cin >> interval;

        std::cout << "\nEnter the number of flows to show in the report: ";
        int showing = 0;
        std::cin >> showing;

        std::cout << "\nEnter the sorting criteria (packets/bytes): ";
        std::string sort_by;
        std::cin >> sort_by;

        while (sort_by != "packets" && sort_by != "bytes") {
            std::cerr << "Invalid sorting criteria. Please enter 'packets' or 'bytes': ";
            std::cin >> sort_by;
        }

        const ReportConfiguration configuration(
            interval,
            showing,
            sort_by == "packets" ? SortCriterion::PACKETS : SortCriterion::BYTES);
        const auto selected_interface =
            interface_manager.selectInterface(interface_choice);

        FlowMonitor monitor(selected_interface, configuration);
        monitor.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
