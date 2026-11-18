#pragma once

#include <optional>
#include <cstddef>
#include <cstdint>
#include "packet_info.hpp"

class PacketParser {
public:
    std::optional<PacketInfo> parse(
        const std::uint8_t* bytes,
        std::size_t caplen,
        std::uint32_t wire_bytes) const;
};
