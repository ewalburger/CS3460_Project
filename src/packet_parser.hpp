#include<optional>
#include <cstddef>
#include <cstdint>

std::optional<std::size_t> parse_headers(const std::uint8_t* bytes, std::size_t caplen);