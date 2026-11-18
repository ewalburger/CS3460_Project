#pragma once

#include <cstdint>
#include <string>
#include <utility>

enum class TransportProtocol { TCP, UDP };

class Packet {
public:
    Packet() = default;

    Packet(std::string source_ip,
           std::string destination_ip,
           std::uint16_t source_port,
           std::uint16_t destination_port,
           TransportProtocol protocol,
           std::uint32_t wire_bytes)
        : source_ip(std::move(source_ip)),
          destination_ip(std::move(destination_ip)),
          source_port(source_port),
          destination_port(destination_port),
          protocol(protocol),
          wire_bytes(wire_bytes) {}

    std::string source_ip;
    std::string destination_ip;
    std::uint16_t source_port{};
    std::uint16_t destination_port{};
    TransportProtocol protocol{};
    std::uint32_t wire_bytes{};
};