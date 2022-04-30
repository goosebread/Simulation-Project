This repository provides a framework for creating a network simulation.
The ParameterServer and ProcessingUnit classes specifically implement a parameter server network topography

To run a simulation, SimInputs.csv must be provided with the following parameters
1: Total Iterations
2: Number of Worker Nodes
3: Bounded Delay Window Size
4: CV for service time gamma distribution
5: random seed

The output file SimOutputs.csv will contain the input rows as well as additional information:
1: Throughput - Iterations done per Adjusted Time Unit
2: Average Utilization - utilization averaged across all workers
3: Adjusted Execution Time - Total execution time in adjusted time units
4: Idle Histogram - The columns starting from this one represent the number of workers that are idle (integers starting from 0).
                    The value represents the freqeuncy.

We use the following definitions for the two time units used:
Normalized Time Units - 1 time unit is the average time it takes for one worker 
                        to calculate the forward pass and backward pass for its contribution to the weight update
Adjusted Time Units - 1/n time units is the average time it takes for one worker (out of n total workers)
                        to calculate the forward pass and backward pass for its contribution to the weight update

The SimLog log file only records the events associated with the last run. The time units are normalized time units


use ./build.sh or ./build.bat to compile
use ./run.sh or ./run.bat to run executable

The .vscode folder is included to facilitate using the gdb debugger if needed. 

Progress Log:
March 9 - most basic simulation set up. 1 server, 1 worker. they schedule and process 5 events
Mar 13 - 1 server many workers
Mar 16 - add logger, random components, queue on PS thinking process
Mar 22 - change to push/pull synchronized framework of gen 1 Parameter servers (removed queue)
Mar 31 - Implement bounded delay model, fix bugs
April 10 - clean up user interface
April 17 - add histogram of idle workers and redefine time units
