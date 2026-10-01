#include<optional>
#include <cstddef>
#include <cstdint>

struct FlowInfo {
    std::size_t transport_offset;
    std::uint16_t src_port;
    std::uint16_t dst_port;
    bool is_tcp; // true if TCP, false if UDP
};
std::optional<FlowInfo> parse_headers(const std::uint8_t* bytes, std::size_t caplen);