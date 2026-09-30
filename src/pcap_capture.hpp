#pragma once
#include <string>
#include <vector>
#include <memory>
#include <pcap/pcap.h>

using PcapHandle = std::unique_ptr<pcap_t, decltype(&pcap_close)>;

std::vector<std::string> get_and_display_interfaces();
PcapHandle open_pcap_handle(const std::string& interface);