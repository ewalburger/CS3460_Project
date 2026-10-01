#include <iostream>
#include <pcap/pcap.h>
#include <optional>
#include "packet_parser.hpp"
#include "packet_info.hpp"
#include <cstdint>

struct FlowInfo {
    std::size_t transport_offset;
    std::uint16_t src_port;
    std::uint16_t dst_port;
    bool is_tcp; // true if TCP, false if UDP
};

std::optional<FlowInfo> parse_headers(const std::uint8_t* bytes, std::size_t caplen) {
    // This function parses the Ethernet and IP headers from a packet.
    // It returns std::nullopt if the packet is too short or invalid.
    // Otherwise, it returns the transport layer offset.

    // Check Ethernet type (Bytes 12-13): 0x0800 indicates IPv4
    std::uint16_t eth_type = (static_cast<std::uint16_t>(bytes[12]) << 8) | bytes[13];
    if (eth_type != 0x0800) return std::nullopt;

    constexpr std::size_t eth = 14;
    if (caplen < eth + 20) return std::nullopt;
    const std::uint8_t* ip_header = bytes + eth;
    const std::uint8_t first = bytes[eth];
    const std::uint8_t version = first >> 4;
    const std::size_t ip_len = (first & 0x0F) * 4;
    if (version != 4 || ip_len < 20 || caplen < eth + ip_len) {
        return std::nullopt;
    }
    const std::size_t transport = eth + ip_len;

    std::uint8_t protocol = ip_header[9];
    if (protocol != 6 && protocol != 17) {
        return std::nullopt;
    }

    //looking at fragment offset
    std::uint16_t frag_field = (static_cast<std::uint16_t>(ip_header[6]) << 8) | ip_header[7];
    std::uint16_t frag_offset = frag_field & 0x1FFF;

    // If fragment offset is non-zero, TCP/UDP headers are missing in this fragment
    if (frag_offset != 0) {
        return std::nullopt;
    }
    std::size_t transport_offset = eth + ip_len;
    bool is_tcp = (protocol == 6);
    std::size_t min_transport_len = is_tcp ? 20 : 8;

<<<<<<< HEAD
    return std::optional<std::size_t>(transport);
}
=======
    // Ensure enough bytes remain for the transport header
    if (caplen < transport_offset + min_transport_len) {
        return std::nullopt;
    }

    // 5. Extract Source and Destination Ports
    const std::uint8_t* transport_header = bytes + transport_offset;
    std::uint16_t src_port = (static_cast<std::uint16_t>(transport_header[0]) << 8) | transport_header[1];
    std::uint16_t dst_port = (static_cast<std::uint16_t>(transport_header[2]) << 8) | transport_header[3];

    return FlowInfo{transport_offset, src_port, dst_port, is_tcp};
}
>>>>>>> dbabdf44b9192585471efe0be8c39faf215b7513
