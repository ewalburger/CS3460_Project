
# Some notes for milestone 2 
Evaluation criteria 4
- sort by packets and sort by bytes?
- use concurrency to continuously get packets without disrupting other processes 

- make separate threads for caputure and report 
- lock/unlock - make sure processes don't read and write at the same time

## Plan
- implement multi threading first 
- then make a flow table

Step 1
- persist means in-memory persistence during the lifetime of our app running. It doesn't mean a database.
- as packets arrive, the packet monitor aggregates them into stats stored inside a collection such as  std::map
- goal is to group individual packets into "flows" to track statistics instead of printing raw packet-by-packet data
- The directional five-tuple (flowkey)
	- five-tuple: standard set of 5 values that uniquely identify a network conversation
	- directional-five-tuple, so direction matters
	- we are provided the exact blueprint for the FlowKey scruct.
		- they key is just the identifier (address/port combo). the counters and timestamps are the stats that go to the keys.
- we will choose std::map<Flowkey, FlowStats> as our flow table
	- must use the operator< function with this std::map. The operator allows us to compare FlowKey objects, which usually aren't comparable because FlowKey is a custom struct
- overall things to do for step 1:
	1. Copy the FlowKey struct into our code
	2. Put the provided comparison operator "operator<" outside of the struct right below it
	3. create a map container: `std::map<FlowKey, FlowStats> flow_table;`
	4. update the flow_table each time a new packet comes in
		1. upon packet arrival extract its five-tuple to build a FlowKey
		2. See if the FlowKey already exists
		3. If it doesn't already exist, create it. If It does exist, update its corresponding FlowStats with packet counts, byte countes, ect.

Step 2
- updating the record
- look at flow table
- update packet and byte totals

Step 3
- this could be the step where we implement multithreading?
- output formatting changes , timing clarification

## Divide and Conquer 
- Multithreading : Jayne
	- split up the capture and updater, diagrma on canvas - use page 26 of concurrency 1 on lecture slides
	- the diagram on page 25 is NOT correct
- Part 1 : Emberly
	- try to have it implement it
- Part 2 : Gannon 
	- try to make sense of what the instructions are actually telling us to do

- next meeting (10/14): research our topic, have pseudocode, attempt to implement it before meeting
