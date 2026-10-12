
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
- choose a flow table
- create a flow table and update it?

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
