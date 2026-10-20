#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <utility>

#include "packet_info.hpp"

class PacketQueue {
public:
    void push(PacketInfo packet) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (closed_) {
                return;
            }
            packets_.push(std::move(packet));
        }
        available_.notify_one();
    }

    std::optional<PacketInfo> pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        available_.wait(lock, [this] {
            return closed_ || !packets_.empty();
        });

        if (packets_.empty()) {
            return std::nullopt;
        }

        PacketInfo packet = std::move(packets_.front());
        packets_.pop();
        return packet;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            closed_ = true;
        }
        available_.notify_all();
    }

private:
    std::queue<PacketInfo> packets_;
    std::mutex mutex_;
    std::condition_variable available_;
    bool closed_ = false;
};
