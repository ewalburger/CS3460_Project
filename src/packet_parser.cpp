#include <iostream>
#include <pcap/pcap.h>
#include <optional>
#include "packet_parser.hpp"
#include <cstdint>

auto parse_headers(const std::uint8_t* bytes, std::size_t caplen) {
    // This function parses the Ethernet and IP headers from a packet.
    // It returns std::nullopt if the packet is too short or invalid.
    // Otherwise, it returns the transport layer offset.
    constexpr std::size_t eth = 14;
    if (caplen < eth + 20) return std::nullopt;
    const std::uint8_t first = bytes[eth];
    const std::uint8_t version = first >> 4;
    const std::size_t ip_len = (first & 0x0F) * 4;
    if (version != 4 || ip_len < 20 || caplen < eth + ip_len) {
        return std::nullopt;
    }
    const std::size_t transport = eth + ip_len;
    return std::optional<std::size_t>(transport);
}
