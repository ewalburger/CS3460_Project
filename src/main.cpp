#include <iostream>
#include <pcap/pcap.h>
#include "pcap_capture.hpp"
 
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
        std::cout << "\nEnter the number of the interface you want to use: ";
        std::cin >> choice;

        // call open_pcap_handle with the chosen interface
        if (choice < 1 || choice > interfaces.size()) {
            std::cerr << "Invalid choice." << std::endl;
            return 1;
        }
        auto pcap_handle = open_pcap_handle(interfaces[choice - 1]);

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
