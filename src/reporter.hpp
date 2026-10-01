#pragma once

#include "packet_info.hpp"

class Reporter {
public:
    void report(const PacketInfo& packet) const;

    std::string summarize(const PacketInfo& packet) const;

private:
    static std::string protocolToString(TransportProtocol protocol);
};