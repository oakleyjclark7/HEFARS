# HEFARS  
**HEXITEC Electric Field and Readout Simulation**

HEFARS processes data from my other Geant4 projects, and takes into account charge transport and electronic readout

Author: Oakley Clark, oakley.clark98@gmail.com, https://www.linkedin.com/in/oakleyjarvisclark/

## Building

> **Note:** Currently tested only on **Windows** using **GCC** and a provided Makefile.

To build the executable:

1. Ensure you have **GCC** and **Make** installed. You can use environments like:
2. Open a terminal or command prompt.
3. Navigate to the project directory.
4. Run:
   ```bash
   make

This will compile the project and produce HEFARS.exe

## Running

1. From a terminal or command prompt, run:
    '''bash
    HEFARS.exe
2. You will be prompted for the path to a run file (.txt) that contains the simulation instructions
3. An example run file is included as EXAMPLE.txt

## Run File

[IN FILE] -> Path to the input CSV containing raw data from Geant4 (must end in .csv)

[HISTOGRAM]
Emin-> Minimum energy (keV)
Emax-> Maximum energy (keV)
Estep-> Bin width (keV) (approximate; program may adjust)

[CHARGE SHARING]
threshold -> Charge-sharing threshold (keV)

[DEAD TIME]
eventrate -> Number of primary incident photons per second