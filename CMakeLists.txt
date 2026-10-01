find_package(PkgConfig REQUIRED)
pkg_check_modules(PCAP REQUIRED IMPORTED_TARGET libpcap)
add_executable(flow_monitor
    src/main.cpp
    src/pcap_capture.cpp
    src/packet_parser.cpp
    src/reporter.cpp
)
target_compile_features(flow_monitor PRIVATE cxx_std_20)
target_link_libraries(flow_monitor PRIVATE PkgConfig::PCAP)