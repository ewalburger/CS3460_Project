#pragma once
#include <string>
#include <vector>
#include <memory>
#include <csignal>
#include "packet_info.hpp"
#include "packet_queue.hpp"
#include <pcap/pcap.h>

using PcapHandle = std::unique_ptr<pcap_t, decltype(&pcap_close)>;

void handle_sigint(int signal);
bool capture_stop_requested();
std::vector<std::string> get_and_display_interfaces();
// Open a pcap handle for the specified network interface and verify that it supports Ethernet capture.
PcapHandle open_pcap_handle(const std::string& interface);
/// Capture and enqueue a specified number of packets.
void capture_loop(PcapHandle& handle, PacketQueue& packets);