# Milestone 1 Design
**GOAL**: Capture live packets safely from one selected interface and convert supported Ethernet/IPv4/TCP/UDP packets into
structured PacketInfo values.

## Select and open an interface 
- Use `pcap_findalldevs()` to find an interface to listen to
- Open the thing you want to look at with pcap_live
- very if it is actually

- starter code is `pcap_open_live()` and `pcap_datalink()`

## Read packets with pcap_next_ex()
- buffer bounds check: verifies if data being read in stays in the allocated size, prevents writing beyond buffer boundaries
- check the right offset >= 0 and right offset+datalength <= buffersize. If either condition fails program throws an error.
- starter code is the building block for this step

## Locate Ehternet, IPv4, and TCP/UDP
- parse the header (?)
- ensure everything is present 
- fragmented IPv4 traffic: when large ip packets are broken into smaller fragments for smaller networks
- piece together fragments 
- fragment offset: specifies starting position of fragment, important for reassembling fragment packets. (sometimes they come in out of order)

## PacketInfo and Output
- summarize all the headers in one line

## Requirements / Evaluation Criteria
1. running with a valid interface starts live capture and clearly prints the selected interface
2. When TCP traffic is generated, the output shows TCP plus source/destination IPv4 addresses and ports 
3. When UDP traffic is generated, the output shows UDP plus source/destination IPv4 addresses and ports (5 points).
4. clean termination

## Current gameplan
- write pseudocode for each step
