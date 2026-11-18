#include "flow_monitor.hpp"

#include <chrono>
#include <condition_variable>
#include <csignal>
#include <exception>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <utility>

FlowMonitor::FlowMonitor(
    std::string interface_name, ReportConfiguration configuration)
    : interface_name_(std::move(interface_name)),
      configuration_(std::move(configuration)) {}

void FlowMonitor::reportFlows() const {
    const auto snapshot = aggregator_.snapshot();
    std::cout << "\nFlow Summary:\n";
    std::cout << formatter_.format(snapshot, configuration_);
}

void FlowMonitor::run() {
    capture_.open(interface_name_);

    if (std::signal(SIGINT, handle_sigint) == SIG_ERR) {
        throw std::runtime_error("Unable to install SIGINT handler.");
    }

    std::cout << "Opened interface: " << interface_name_ << std::endl;
    std::exception_ptr capture_error;
    const auto report_interval =
        std::chrono::seconds(configuration_.interval());
    std::mutex report_mutex;
    std::condition_variable report_condition;
    bool stop_reporting = false;

    std::thread capture_thread([&] {
        try {
            capture_.capturePackets(packet_queue_);
        } catch (...) {
            capture_error = std::current_exception();
        }
        packet_queue_.close();
    });

    std::thread reporter_thread([&] {
        std::unique_lock<std::mutex> lock(report_mutex);
        while (!stop_reporting) {
            const auto deadline =
                std::chrono::steady_clock::now() + report_interval;
            if (report_condition.wait_until(lock, deadline, [&] {
                    if (stop_reporting) {
                        std::cout << "\nStopping flow reporting..." << std::endl;
                    }
                    return stop_reporting;
                })) {
                break;
            }

            lock.unlock();
            reportFlows();
            lock.lock();
        }
    });

    std::thread update_thread([&] {
        while (const auto packet = packet_queue_.pop()) {
            aggregator_.update(*packet);
        }
    });

    capture_thread.join();
    update_thread.join();

    {
        std::lock_guard<std::mutex> lock(report_mutex);
        stop_reporting = true;
    }
    report_condition.notify_one();
    reporter_thread.join();

    if (capture_error) {
        std::rethrow_exception(capture_error);
    }

    reportFlows();
}
