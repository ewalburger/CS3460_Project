
# Some notes for milestone 2 

## Plan 
- implement multi threading first 
- then make a flow table

Step 1
- choose a flow table
- create a flow table and update it?

Step 2
- updating the record
- look at flow table
- update packet and byte totals

Step 3
- this could be the step where we implement multithreading?
- output formatting changes , timing clarification

Some Notes on Multithreading
Program will be split into three threads 
- capturer
- updater 
- reporter

Shared data
- Capturer and updater will share a locked queue
- Updater and reporter will share a locked table

#### Psuedocode for step 3
```
// ---- Shared state ----
PacketQueue:
    queue of packets
    lock
    signal (wakes the consumer when data arrives)

FlowTable:
    table data
    lock

running = true


// ---- Thread 1: Capture ----
function CaptureThread():
    while running:
        packet = capturePacket()

        acquire queueLock
            queue.push(packet)
        release queueLock

        signal Updater that a packet is available

    signal Updater one last time (so it can wake up and exit)


// ---- Thread 2: Updater ----
function UpdaterThread():
    loop forever:
        acquire queueLock
            wait until (queue is not empty) OR (running is false)

            if queue is empty AND running is false:
                release queueLock
                exit loop

            packet = queue.pop()
        release queueLock

        acquire flowTableLock
            updateFlowTable(table, packet)
        release flowTableLock


// ---- Thread 3: Reporter ----
function ReporterThread():
    while running:
        sleep 1 second

        acquire flowTableLock
            printReport(table)
        release flowTableLock


// ---- Main ----
function main():
    start CaptureThread
    start UpdaterThread
    start ReporterThread

    wait for shutdown signal
    running = false

    wait for all three threads to finish
```


# Meeting Notes

## Meeting on 10/7
Evaluation criteria 4
- sort by packets and sort by bytes?
- use concurrency to continuously get packets without disrupting other processes 

- make separate threads for caputure and report 
- lock/unlock - make sure processes don't read and write at the same time

## Divide and Conquer 
- Multithreading : Jayne
	- split up the capture and updater, diagrma on canvas - use page 26 of concurrency 1 on lecture slides
	- the diagram on page 25 is NOT correct
- Part 1 : Emberly
	- try to have it implemented
- Part 2 : Gannon 
	- try to make sense of what the instructions are actually telling us to do

- next meeting (10/14): research our topic, have pseudocode, attempt to implement it before meeting

## Meeting On 10/14
So far: done minimal research. Still need to begin implementation