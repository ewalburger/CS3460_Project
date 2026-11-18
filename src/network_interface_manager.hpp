#pragma once

#include <cstddef>
#include <string>
#include <vector>

class NetworkInterfaceManager {
public:
    std::vector<std::string> discoverInterfaces();
    void displayInterfaces() const;
    std::string selectInterface(std::size_t selection) const;

private:
    std::vector<std::string> interfaces_;
    std::vector<std::string> descriptions_;
};