# Milestone 1 Design
**GOAL**: Capture live packets safely from one selected interface and convert supported Ethernet/IPv4/TCP/UDP packets into
structured PacketInfo values.

## Step 0 - Cmake configuration
- replace our cmake config file with the one provided in the starter code
- ensure pkg config is installed

## File Structure
- main.cpp
- pcap_capture.cpp
- packet_parser.cpp

- all files contained in executable called flow_monitor

## Step 1 - Select and open an interface 
- Use `pcap_findalldevs()` to find an interface to listen to
- Open the thing you want to look at with `pcap_live`
- very if it is actually

- starter code is `pcap_open_live()` and `pcap_datalink()`

After looking at this starter code..
- need to fill in the blank of where the comment `/* report errbuf */` is located
	- this placeholder is where we would print/log `errbuf` and presumable return/exit sincle nothing below is safe to run without a valid handle.
- need to modify starter code to accept ethernet, ip4v, TCP, and UDP (located at `pcap_datalink(...) != DLT_EN10MB`)

### What we have done for step 1
- starter code was missing import statements, so we added those
- we already linked the headers in step 0 in the CMake file we edited.
- simple error handling - decided to throw exception in case of error


#### Questions about this step
- are we allowed to use `auto` return type or should we specify return type?

## Step 2: Read packets with pcap_next_ex()
- buffer bounds check: verifies if data being read in stays in the allocated size, prevents writing beyond buffer boundaries
- check the right offset >= 0 and right offset+datalength <= buffersize. If either condition fails program throws an error.
- starter code is the building block for this step

Here is some pseudocode for how the loop should look

```

FUNCTION run_capture_loop(pcap_handle, parser, flow_table):

    WHILE running_flag IS true:

        header, bytes, rc = pcap_next_ex(pcap_handle)

        IF rc == 1:                     # packet successfully captured
            # bytes/header are ONLY valid until the next pcap_next_ex call
            # -> must fully parse and copy out needed data right now

            packet = parser.parse(bytes, header.caplen, header.len)
            #   - use caplen for every bounds check inside parse()
            #   - use len only as metadata (e.g. detect truncation)
            #   - parse() returns an OWNED struct/object, no pointers
            #     back into bytes

            IF packet IS valid:
                flow_table.update(packet)
            ELSE:
                increment malformed_packet_count   # optional stat

        ELSE IF rc == 0:                # timeout, no packet available
            CONTINUE                    # just loop again

        ELSE IF rc == -1:               # capture error
            log_error(pcap_geterr(pcap_handle))
            BREAK                       # stop capturing

        ELSE IF rc == -2:               # end of offline capture file
            BREAK                       # no more packets will come

        # --- periodic housekeeping (not tied to packet arrival) ---
        IF time_since_last_sweep >= sweep_interval:
            flow_table.expire_idle_flows(timeout_threshold)
            flow_table.report()         # print/export current state
            reset sweep_timer

    END WHILE

    flow_table.report()                 # final flush on exit
```

## Step 3: Locate Ehternet, IPv4, and TCP/UDP
- parse the header (?)
- ensure everything is present 
- fragmented IPv4 traffic: when large ip packets are broken into smaller fragments for smaller networks
- piece together fragments 
- fragment offset: specifies starting position of fragment, important for reassembling fragment packets. (sometimes they come in out of order)

## Step 4: PacketInfo and Output
- summarize all the headers in one line

## Requirements / Evaluation Criteria
1. running with a valid interface starts live capture and clearly prints the selected interface
2. When TCP traffic is generated, the output shows TCP plus source/destination IPv4 addresses and ports 
3. When UDP traffic is generated, the output shows UDP plus source/destination IPv4 addresses and ports (5 points).
4. clean termination

## Current Gameplan
- write pseudocode for each step

## Current Questions
- how should we manage dependencies? do we need a virtual environment? or does the build script do it for us?
 (put it in the readme to install dependencies for now)
