#pragma once
#include <string>
#include <vector>
#include <memory>
#include <pcap/pcap.h>

using PcapHandle = std::unique_ptr<pcap_t, decltype(&pcap_close)>;

std::vector<std::string> get_and_display_interfaces();
// Open a pcap handle for the specified network interface and verify that it supports Ethernet capture.
PcapHandle open_pcap_handle(const std::string& interface);
/// Capture and process a specified number of packets.
void capture_loop(PcapHandle& handle, int packet_count);