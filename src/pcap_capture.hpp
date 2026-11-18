#pragma once

#include <csignal>
#include <memory>
#include <string>

#include "packet_info.hpp"
#include "packet_queue.hpp"
#include <pcap/pcap.h>

void handle_sigint(int signal);

class PcapCapture {
public:
    void open(const std::string& interface_name);
    void capturePackets(PacketQueue& packets);
    void stop();

private:
    using PcapHandle = std::unique_ptr<pcap_t, decltype(&pcap_close)>;
    PcapHandle handle_{nullptr, &pcap_close};
};