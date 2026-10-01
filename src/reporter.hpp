#pragma once

#include "packet_info.hpp"

class Reporter {
public:
    // Prints a single packet's info to stdout
    void report(const PacketInfo& packet) const;

    // Returns a one-line string summary instead of printing directly
    std::string summarize(const PacketInfo& packet) const;

private:
    // Helper to turn the enum into readable text
    static std::string protocolToString(TransportProtocol protocol);
};