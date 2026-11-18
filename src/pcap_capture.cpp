#include <pcap/pcap.h>
#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>
#include "pcap_capture.hpp"
#include <optional>
#include "packet_parser.hpp"
#include "flow_aggregator.hpp"

namespace {
volatile std::sig_atomic_t stop_requested = 0;
}

void handle_sigint(int) {
    stop_requested = 1;
}

bool capture_stop_requested() {
    return stop_requested != 0;
}

PcapHandle open_pcap_handle(const std::string& interface) { 
    // open the interface and verify the datalink

    char errbuf[PCAP_ERRBUF_SIZE]{};
    pcap_t* raw = pcap_open_live(interface.c_str(), 65535, 0, 1000, errbuf);
    if (!raw) { throw std::runtime_error(std::string("pcap_open_live failed: ") + errbuf); }

    using PcapHandle = std::unique_ptr<pcap_t, decltype(&pcap_close)>;
    PcapHandle handle(raw, &pcap_close);

    if (pcap_datalink(handle.get()) != DLT_EN10MB) {
        throw std::runtime_error("this milestone supports Ethernet capture only");
    }

    return handle; 
}




void capture_loop(PcapHandle& handle, PacketQueue& packets) {
    char errbuf[PCAP_ERRBUF_SIZE]{};
    if (pcap_setnonblock(handle.get(), 1, errbuf) != 0) {
        throw std::runtime_error(errbuf);
    }

    while (!capture_stop_requested()) {
        struct pcap_pkthdr* header = nullptr;
        const u_char* bytes = nullptr;

        int rc = pcap_next_ex(handle.get(), &header, &bytes);

        if (rc == 1) {

            std::size_t capturedLen = header->caplen;

            std::size_t wireLen = header->len;
            
            // call parser
            std::optional<PacketInfo> packet = parse_headers(bytes, capturedLen, wireLen);

            if (packet.has_value()) {
                packets.push(std::move(*packet));
            }

        } else if (rc == 0) {
            // timeout; continue
            continue;
        } else if (rc == -1) {
            // capture error
            throw std::runtime_error(std::string("pcap_next_ex failed: ") + pcap_geterr(handle.get()));
        }

    }
}
