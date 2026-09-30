#include <iostream>
#include <pcap/pcap.h>
#include "pcap_capture.cpp"
 
int main()
{
    try {
        auto interfaces = get_and_display_interfaces();

        if (interfaces.empty()) {
            std::cerr << "No network interfaces found." << std::endl;
            return 1;
        }
        // user to choose an interface
        // call open_pcap_handle with the chosen interface

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }


    std::cout << "Hello, world!" << std::endl;
    return 0;
}
