#include <pcap/pcap.h>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "pcap_capture.hpp"
#include "packet_parser.hpp"

std::vector<std::string> get_and_display_interfaces() {
    // use pcap_findalldevs() to list all available network interfaces
    pcap_if_t *alldevs = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE]{};

    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        throw std::runtime_error(std::string("Error finding devices: ") + errbuf);
    }

    std::vector<std::string> interfaces;
    int index = 1;

    std::cout << "Available Network Interfaces:\n";
    for (pcap_if_t* dev = alldevs; dev != nullptr; dev = dev->next) {
        std::string name = dev->name ? dev->name : "unknown";
        std::string desc = dev->description ? dev->description : "No description";

        std::cout << "  [" << index++ << "] " << name << " (" << desc << ")\n";
        interfaces.push_back(name);
    }

    pcap_freealldevs(alldevs);
    return interfaces;
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

    return handle; // ?
}


void capture_loop(PcapHandle& handle, int packet_count) {
    int packets_processed = 0;

    while (packet_count <= 0 || packets_processed < packet_count) {
        struct pcap_pkthdr* header = nullptr;
        const u_char* bytes = nullptr;

        int rc = pcap_next_ex(handle.get(), &header, &bytes);

        if (rc == 1) {
            // call parser

            // caplen = number of bytes actually captured into the buffer.
            // This is the ONLY value safe to use for bounds/indexing into `bytes`.
            std::size_t capturedLen = header->caplen;

            // len = original length of the packet on the wire. May be LARGER
            // than caplen if the packet was truncated (e.g. by snaplen).
            // Safe to print/log, but never use it to index into `bytes`.
            std::size_t wireLen = header->len;

			parse_headers(bytes, capturedLen);

            std::cout << "Captured a packet with length: " << wireLen << std::endl;
            std::cout << "Packet Header: ts_sec=" << header->ts.tv_sec;
            std::cout << "Packet Header: ts_sec=" << header->ts.tv_sec
                       << " ts_usec=" << header->ts.tv_usec
                       << " caplen=" << capturedLen
                       << " len=" << wireLen << std::endl;

            // NOTE: `bytes` is only valid until the next call to pcap_next_ex.
            // libpcap owns and may reuse/overwrite this buffer afterward, so
            // nothing here should store `bytes` itself -- once parsing is
            // added, it must happen synchronously here, copying out whatever
            // data needs to be kept.

            ++packets_processed;
        } else if (rc == 0) {
            // timeout; continue
            continue;
        } else if (rc == -1) {
            // capture error
            throw std::runtime_error(std::string("pcap_next_ex failed: ") + pcap_geterr(handle.get()));
        }
        
    }
}

/*
cap_pkthdr* header = nullptr;
const u_char* bytes = nullptr;
int rc = pcap_next_ex(handle.get(), &header, &bytes);
CS 3460 Modern C++ | Project 2: Live Network Flow Monitor
if (rc == 1) {
auto packet = parser.parse(bytes, header->caplen, header->len);
} else if (rc == 0) {
// timeout; continue
} else if (rc == -1) {
// capture error


*/
