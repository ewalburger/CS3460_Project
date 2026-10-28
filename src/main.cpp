#include <iostream>
#include <pcap/pcap.h>
#include "pcap_capture.hpp"
#include "reporter.hpp"
#include "flow_aggregator.hpp"
#include <condition_variable>
#include <exception>
#include <mutex>
#include <thread>
#include <chrono>

 
int main()
{
    try {
        auto interfaces = get_and_display_interfaces();

        if (interfaces.empty()) {
            std::cerr << "No network interfaces found." << std::endl;
            return 1;
        }
        // user needs to choose an interface
        size_t interface_choice = 0;
        std::cout << "\nEnter the interface number you want to use: ";
        std::cin >> interface_choice;

        std::cout << "\nEnter the interval in seconds for flow reporting: ";
        int interval = 0;
        std::cin >> interval;
        if (interval < 1 || interval > 2) {
            std::cerr << "Report interval must be 1 or 2 seconds." << std::endl;
            return 1;
        }

        std::cout << "\nEnter the number of flows to show in the report: ";
        int showing = 0;
        std::cin >> showing;


        std::cout << "\nEnter the sorting criteria (packets/bytes): ";
        std::string sortBy;
        std::cin >> sortBy;

        while (sortBy != "packets" && sortBy != "bytes") {
            std::cerr << "Invalid sorting criteria. Please enter 'packets' or 'bytes': ";
            std::cin >> sortBy;
        }

        // call open_pcap_handle with the chosen interface
        if (interface_choice < 1 || interface_choice > interfaces.size()) {
            std::cerr << "Invalid choice." << std::endl;
            return 1;
        }


        auto pcap_handle = open_pcap_handle(interfaces[interface_choice - 1]);

        std::cout << "Opened interface: " << interfaces[interface_choice - 1] << std::endl;
        PacketQueue packets;
        std::exception_ptr capture_error;
        Reporter reporter;
        FlowAggregator aggregator;
        const auto report_interval = std::chrono::seconds(interval);
        std::mutex report_mutex;
        std::condition_variable report_condition;
        bool stop_reporting = false;
        const auto report_flow_table = [&] {
            const auto snapshot = aggregator.snapshot();
            std::cout << "\nFlow Summary:\n";
            std::cout << reporter.summarize(snapshot, interval, showing, sortBy);
        };

        std::thread capture_thread([&] {
            try {
                capture_loop(pcap_handle, packets);
            } catch (...) {
                capture_error = std::current_exception();
            }
            packets.close();

        });

        std::thread reporter_thread([&] {
            std::unique_lock<std::mutex> lock(report_mutex);
            while (!stop_reporting) {
                const auto deadline =
                    std::chrono::steady_clock::now() + report_interval;
                if (report_condition.wait_until(lock, deadline, [&] {
                        return stop_reporting;
                    })) {
                    break;
                }

                lock.unlock();
                report_flow_table();
                lock.lock();
            }
        });

        std::thread update_thread([&] {
            while (const auto packet = packets.pop()) {
                aggregator.update(*packet);
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

        report_flow_table();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
