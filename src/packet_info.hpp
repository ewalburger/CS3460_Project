#pragma once

#include <cstdint>
#include <string>

enum class TransportProtocol { TCP, UDP };

struct PacketInfo {
    std::string source_ip;
    std::string destination_ip;
    std::uint16_t source_port{};
    std::uint16_t destination_port{};
    TransportProtocol protocol{};
    std::uint32_t wire_bytes{};
};