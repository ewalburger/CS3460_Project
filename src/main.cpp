#include <iostream>
#include "pcap_capture.hpp"
#include "network_interface_manager.hpp"
#include "reporter.hpp"
#include "flow_aggregator.hpp"
#include "report_configuration.hpp"
#include <condition_variable>
#include <exception>
#include <mutex>
#include <thread>
#include <chrono>
#include <csignal>

 
int main()
{
    try {
        NetworkInterfaceManager interface_manager;
        const auto interfaces = interface_manager.discoverInterfaces();

        if (interfaces.empty()) {
            std::cerr << "No network interfaces found." << std::endl;
            return 1;
        }
        interface_manager.displayInterfaces();
        // user needs to choose an interface
        size_t interface_choice = 0;
        std::cout << "\nEnter the interface number you want to use: ";
        std::cin >> interface_choice;

        std::cout << "\nEnter the interval in seconds for flow reporting: ";
        int interval = 0;
        std::cin >> interval;

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

        const ReportConfiguration report_configuration(
            interval,
            showing,
            sortBy == "packets" ? SortCriterion::PACKETS : SortCriterion::BYTES);
        const std::string configured_sort_by =
            report_configuration.sortCriterion() == SortCriterion::PACKETS
                ? "packets"
                : "bytes";

        // call open_pcap_handle with the chosen interface
        const auto selected_interface =
            interface_manager.selectInterface(interface_choice);
        PcapCapture capture;
        capture.open(selected_interface);

        if (std::signal(SIGINT, handle_sigint) == SIG_ERR) {
            throw std::runtime_error("Unable to install SIGINT handler.");
        }

        std::cout << "Opened interface: " << selected_interface << std::endl;
        PacketQueue packets;
        std::exception_ptr capture_error;
        Reporter reporter;
        FlowAggregator aggregator;
        const auto report_interval =
            std::chrono::seconds(report_configuration.interval());
        std::mutex report_mutex;
        std::condition_variable report_condition;
        bool stop_reporting = false;
        const auto report_flow_table = [&] {
            const auto snapshot = aggregator.snapshot();
            std::cout << "\nFlow Summary:\n";
            std::cout << reporter.summarize(
                snapshot,
                report_configuration.interval(),
                report_configuration.numberToShow(),
                configured_sort_by);
        };

        std::thread capture_thread([&] {
            try {
                capture.capturePackets(packets);
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
                    // print that the reporting is about to stop
                    if (stop_reporting) {
                        std::cout << "\nStopping flow reporting..." << std::endl;
                    }
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
