#pragma once

#include <optional>
#include <cstddef>
#include <cstdint>
#include "packet_info.hpp"

struct FlowKey {
    TransportProtocol protocol{}; 
    std::string source_ip;
    std::uint16_t source_port{};
    std::string destination_ip;
    std::uint16_t destination_port{};
};

std::optional<PacketInfo> parse_headers(
    const std::uint8_t* bytes,
    std::size_t caplen,
    std::uint32_t wire_bytes);
