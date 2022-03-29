some code for this project for this class i'm in

we can make the cmake fancier if need be but we're probably fine just starting with vanilla cpp

for now, the .vscode folder will be included to facilitate the setting up of build environments if yall want to try running it

-Alex Yeh, March 6 2022


use ./build.sh to compile
use ./run.sh to run whatever's in main

use vscode's launch task to debug

install dependencies?
sudo apt install libopenblas-dev liblapack-dev libboost-dev libarmadillo-dev 

Progress Log:
March 9 - most basic simulation set up. 1 server, 1 worker. they schedule and process 5 events
Mar 13 - 1 server many workers
Mar 16 - add logger, random components, queue on PS thinking process
Mar 22 - change to push/pull synchronized framework of gen 1 Parameter servers, comments could use work though