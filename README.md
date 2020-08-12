# simsim

## INSTALL and RUN
1. Download files from this repository
```
$ git clone https://github.com/lsoros/simsim.git
```
2. Run ./compile.sh to compile code (will also automatically run Novelty experiment as default)
```
$ ./compile.sh
```

## RUN EXPERIMENT

### Simsim.cpp
Main experiment code. Runs the novelty search experiment or the hillclimber experiment as designated by command line argument
```
$ ./simsim novelty        # runs the novelty experiment (default)
$ ./simsim hillclimber    # runs the hillclimber experiment
```

## FILE DESCRIPTIONS

### Simulation.cpp
Functions for running a simulation on a Sim living in a house. 
Includes SimSim agent behavior algorithm as defined in the paper

### Novelty.cpp
Creates a novelty search with local competition (using average KNN distance) and minimum criteria for houses generated.
The values for minimum novelty distance, K nearest neighbors, and minimum sim fitness can be modified in the `includes/simsim_func.h` file

### simObjectList.txt
List of objects that can be placed in a house for the Simsim environment.
Each object per line is in the form (comma separated):
_name, hunger, hygeine, bladder, energy, social, fun_

### sim_objects.cpp
Extended class functions for the SimSim environment.
Includes toJSON definitions for each class, object need ranking selection, and room ASCII map generation function

### includes/class_def.h 
Definition of classes and functions within the SimSim environment. 

