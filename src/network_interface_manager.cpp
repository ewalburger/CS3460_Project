#include "network_interface_manager.hpp"

#include <iostream>
#include <pcap/pcap.h>
#include <stdexcept>
#include <utility>

std::vector<std::string> NetworkInterfaceManager::discoverInterfaces() {
    pcap_if_t* all_devices = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE]{};

    if (pcap_findalldevs(&all_devices, errbuf) == -1) {
        throw std::runtime_error(std::string("Error finding devices: ") + errbuf);
    }

    interfaces_.clear();
    descriptions_.clear();
    for (pcap_if_t* dev = all_devices; dev != nullptr; dev = dev->next) {
        std::string name = dev->name ? dev->name : "unknown";
        interfaces_.push_back(std::move(name));
        descriptions_.push_back(
            dev->description ? dev->description : "No description");
    }

    pcap_freealldevs(all_devices);
    return interfaces_;
}

void NetworkInterfaceManager::displayInterfaces() const {
    std::cout << "Available Network Interfaces:\n";
    for (std::size_t index = 0; index < interfaces_.size(); ++index) {
        std::cout << "  [" << index + 1 << "] " << interfaces_[index] << " ("
                  << descriptions_[index] << ")\n";
    }
}

std::string NetworkInterfaceManager::selectInterface(std::size_t selection) const {
    if (selection < 1 || selection > interfaces_.size()) {
        throw std::out_of_range("Invalid interface choice.");
    }

    return interfaces_[selection - 1];
}