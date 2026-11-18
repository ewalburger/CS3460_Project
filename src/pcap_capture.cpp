#include "pcap_capture.hpp"

#include <pcap/pcap.h>
#include <stdexcept>
#include <utility>

namespace {
volatile std::sig_atomic_t stop_requested = 0;
}

void handle_sigint(int) {
    stop_requested = 1;
}

void PcapCapture::open(const std::string& interface_name) {
    char errbuf[PCAP_ERRBUF_SIZE]{};
    pcap_t* raw = pcap_open_live(interface_name.c_str(), 65535, 0, 1000, errbuf);
    if (!raw) {
        throw std::runtime_error(std::string("pcap_open_live failed: ") + errbuf);
    }

    PcapHandle handle(raw, &pcap_close);
    if (pcap_datalink(handle.get()) != DLT_EN10MB) {
        throw std::runtime_error("this milestone supports Ethernet capture only");
    }

    stop_requested = 0;
    handle_ = std::move(handle);
}

void PcapCapture::capturePackets(PacketQueue& packets) {
    if (!handle_) {
        throw std::runtime_error("pcap capture is not open");
    }

    char errbuf[PCAP_ERRBUF_SIZE]{};
    if (pcap_setnonblock(handle_.get(), 1, errbuf) != 0) {
        throw std::runtime_error(errbuf);
    }

    while (stop_requested == 0) {
        struct pcap_pkthdr* header = nullptr;
        const u_char* bytes = nullptr;

        int rc = pcap_next_ex(handle_.get(), &header, &bytes);

        if (rc == 1) {
            std::optional<PacketInfo> packet =
                parser_.parse(bytes, header->caplen, header->len);

            if (packet.has_value()) {
                packets.push(std::move(*packet));
            }

        } else if (rc == 0) {
            // timeout; continue
            continue;
        } else if (rc == -1) {
            throw std::runtime_error(
                std::string("pcap_next_ex failed: ") + pcap_geterr(handle_.get()));
        }
    }
}

void PcapCapture::stop() {
    stop_requested = 1;
}
