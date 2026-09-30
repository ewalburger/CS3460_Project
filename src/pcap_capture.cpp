#include <pcap/pcap.h>
#include <memory>
#include <string>
#include <stdexcept>

auto open_pcap_handle(const std::string& interface) { 
    // use pcap_findalldevs() to list all available network interfaces

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