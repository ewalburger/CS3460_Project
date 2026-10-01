#pragma once

#include <optional>
#include <cstddef>
#include <cstdint>
#include "packet_info.hpp"

std::optional<PacketInfo> parse_headers(
    const std::uint8_t* bytes,
    std::size_t caplen,
    std::uint32_t wire_bytes);