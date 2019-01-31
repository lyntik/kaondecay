#!/bin/sh

# prerequirements:
# - cmake is installed
# - Geant4 is installed and properly configured 
# - ROOT is installed and properly configured 

mkdir -p build && cd build
cmake ..
make -j`grep -c processor /proc/cpuinfo`
./KaonDecay run.mac

# visualize results
#./printEvents.py
#./plotHisto.py

