This repository provides a framework for creating a network simulation.
The ParameterServer and ProcessingUnit classes specifically implement a parameter server network topography

To run a simulation, SimInputs.csv must be provided with the following parameters
1: Total Iterations
2: Number of Worker Nodes
3: Bounded Delay Window Size
4: CV for service time gamma distribution
5: random seed

The output normalized throughput and average utilization will be sent to the file SimOutputs.csv

use ./build.sh or ./build.bat to compile
use ./run.sh or ./run.bat to run executable

The .vscode folder is included to facilitate using the gdb debugger if needed. 

Progress Log:
March 9 - most basic simulation set up. 1 server, 1 worker. they schedule and process 5 events
Mar 13 - 1 server many workers
Mar 16 - add logger, random components, queue on PS thinking process
Mar 22 - change to push/pull synchronized framework of gen 1 Parameter servers, comments could use work though
Mar 31 - Implement bounded delay model, fix bugs
April 10 - clean up user interface