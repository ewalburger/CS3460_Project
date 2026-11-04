# 1. Requirements Analysis
- [x] descirbe what the system should do
- [ ] make each requirement clear and testable
- [x] use requirement IDs (R1, R2, and R3)

This system captures live network flow for TCP and UDP packets. The program prints off live flow reports in a specified interval. Flow is sorted by number of packets or number of bytes specified by the user. The system uses threads to maintain persistent flow records.

| ID | Requirement |
| :------ | :------- |
| R1 | capture flow packets as they become available |
| R2 | allow the user to select and open an interface |
| R3 | report TCP or UDP (differenciate between them) |
| R4 | interrupting the program stops the program clearly without abnormal termination |
| R5 | user selects interval, reports every interval |
| R6 | each report shows one row per directional 5 tuple (flow) | 
| R7 | user selects byte sort mode or packet sort mode and sorts in descending order by # of packets or # of bytes |
| R8 | byte statistics and packet reporting are persistent accross reports (updates instead of new) |
| R9 | user selects # of displayed flow rows |


# 2. System Design
Describe how you plan to organize your project using C++ classes. Design should include
- [ ] major classes: explain the responsibility, important data members, and main functions/interfaces of each class
- [ ] class relationships: provide a UML class diagram or equivalent diagram showing how the classes are related (composition, inheritance, or usage)
- [ ] class interactions: explain how the classes work together to implement the major system functionality 

## Major Classes
Our existing program is already partially object-oriented: PacketQueue, FlowAggregator, and Reporter are classes, while PacketInfo, FlowKey, and FlowStats are data structures. The main opportunity is to move responsibilities currently spread across free functions and main.cpp into cohesive classes.

Suggested classes
|Class	 | Responsibility	| Existing code to move | Requirements Satisfied |
| :------ | :-------: | :------: | :------: |
| NetworkInterfaceManager	| Discover interfaces, display them, validate the user’s selection	| get_and_display_interfaces() | R2 | 
| PcapCapture	| Own the pcap_t handle and capture packets |	open_pcap_handle(), capture_loop() | R1, R3 |
| PacketParser | Convert raw Ethernet/IP/TCP/UDP bytes into PacketInfo |	parse_headers() | R3 (this gathers info used in later requirements) |
| Packet |	Represent a parsed packet with addresses, ports, protocol, and size	| Replace or wrap PacketInfo | formats info for later requirements |
| FlowRow |	Represent one communication flow and update its statistics | Combine FlowKey and FlowStats | R6, R8 | 
| FlowAggregator	| Store flows, update them, and produce snapshots |	Keep existing FlowAggregator | R6, R8 |
| ReportConfiguration	| Store interval, number of flows, and sorting choice	| Input variables in main() | R5, R6, R7, R9 |
| ReportFormatter	| Sort and format flow data for display	| Keep existing Reporter | R3, R5, R6, R7, R9 |
| FlowMonitor	| Coordinate capture, queue processing, aggregation, and reporting threads | Most of main.cpp | R4, covers all requirements |

Describe how each class satisfies requirements listed. (follow 2.2 in example requirements) A couple sentences for each of these and how they interact.

## UML diagram 
```mermaid
classDiagram
    class NetworkInterfaceManager {
        +discoverInterfaces() vector~string~
        +displayInterfaces()
        +selectInterface() string
    }


    class PcapCapture {
        -PcapHandle handle
        -PacketParser parser
        +open(interfaceName)
        +capturePackets(queue)
        +stop()
    }

    class PacketParser {
        +parse(bytes, caplen, wireBytes) optional~Packet~
    }

    class Packet {
        +sourceIp string
        +destinationIp string
        +sourcePort uint16
        +destinationPort uint16
        +protocol TransportProtocol
        +wireBytes uint32
    }

    class PacketQueue {
        +push(packet)
        +pop() optional~Packet~
        +close()
    }

    class FlowRow {
        +FlowKey key
        +FlowStats stats
        +recordPacket(bytes)
    }

    class FlowAggregator {
        -flows map
        +update(packet)
        +snapshot() FlowSnapshot
    }

    class ReportConfiguration {
        +interval int
        +numberToShow int
        +sortCriterion SortCriterion
    }

    class ReportFormatter {
        +sortByPackets(snapshot)
        +sortByBytes(snapshot)
        +format(snapshot, configuration) string
    }

    class FlowMonitor {
        -PcapCapture capture
        -PacketQueue packetQueue
        -FlowAggregator aggregator
        -ReportFormatter formatter
        -ReportConfiguration configuration
        +run()
        -capturePackets()
        -aggregatePackets()
        -reportFlows()
    }

    NetworkInterfaceManager ..> PcapCapture : selects interface
    PcapCapture *-- PacketParser : owns
    PcapCapture --> PacketQueue : pushes packets
    PacketParser ..> Packet : creates
    PacketQueue o-- Packet : stores
    FlowMonitor *-- PcapCapture
    FlowMonitor *-- PacketQueue
    FlowMonitor *-- FlowAggregator
    FlowMonitor *-- ReportFormatter
    FlowMonitor *-- ReportConfiguration
    FlowAggregator *-- FlowRow : stores
    FlowAggregator ..> Packet : processes
    ReportFormatter ..> FlowRow : formats
    ReportFormatter ..> ReportConfiguration : uses

```

**focus on explaining how your design satisfies these requirements, rather than simply listing classes and functions**

# 3. Test Design
Design tests to verify the major requirements and system functionality. For each test, provide:
- [ ] target: the requirement or component being tested
- [ ] input/setup: the input or conditions required
- [ ] expected result: the expected behaivior or output.
[ ] include normal cases and important error/edge cases.
- [ ] briefly explain how your planned tests covered the major requirements.
**tests only need to be designed in this milestone, they will be implemented and executed in the final milestone**

Each Test name corresponds with requirement id
| ID  |Test | Classes involved | Expected result |
|---|---|---|---|
| R1 | Provide several packets through a fake capture source and verify they are processed as they become available. | `PcapCapture`, `PacketParser`, `PacketQueue`, `FlowMonitor` | Each available packet is parsed, queued, and aggregated without waiting for the capture to end. |
| R2 | Supply a list of interfaces, select a valid interface, and attempt to open it. Also test invalid selections. | `NetworkInterfaceManager`, `PcapCapture` | A valid interface opens successfully; invalid choices produce a clear error without crashing. |
| R3 | Create one TCP packet and one UDP packet with the same addresses and ports. | `PacketParser`, `Packet`, `FlowAggregator`, `ReportFormatter` | The packets are classified correctly and appear as separate TCP and UDP flows. |
| R4 | Trigger the stop mechanism, simulating `SIGINT`, while capture and reporting threads are running. | `PcapCapture`, `FlowMonitor`, `PacketQueue` | Capture stops, the queue closes, worker threads join, and the program exits normally with a clear message. |
| R5 | Configure a one-second reporting interval and measure report timestamps. | `ReportConfiguration`, `FlowMonitor`, `ReportFormatter` | Reports are produced approximately once per configured interval. |
| R6 | Send packets for two directional flows: A → B and B → A. | `Packet`, `FlowRow`, `FlowAggregator` | Two rows are produced because reversed source/destination fields represent different flows. |
| R7 | Add flows with known packet and byte totals, then test both sort modes. | `FlowAggregator`, `ReportFormatter` | Packet mode sorts descending by packet count; byte mode sorts descending by byte count. |
| R8 | Add packets, take a snapshot, add more packets, and take another snapshot. | `FlowAggregator`, `FlowRow` | Existing flow totals increase rather than being reset or duplicated. |
| R9 | Configure the report to show 1, 3, or more rows. | `ReportConfiguration`, `ReportFormatter` | The report contains no more than the requested number of rows. |

# This document is unfinished
- elaborate more on class info and test info?
- emberly takes test design section, make it look similar to example
- jayne and gannon split up part 2
- everyone go through document and check for mistakes


















