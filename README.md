# Power Quality Waveform Analyser

This project is a C program for analysing 3-phase power quality waveform data from a CSV file.

## Features
- Loads waveform data from CSV
- Computes RMS voltage for each phase
- Computes peak-to-peak voltage
- Computes DC offset
- Detects clipped samples
- Checks RMS compliance against nominal voltage
- Computes mean frequency, power factor, THD, and line current
- Writes results to `results.txt`

## Build in CLion
Open the project in CLion and build the CMake target.

## Run
The program requires a CSV filename as a command-line argument:

`power_quality_log.csv`

## Output
The program writes a report to:

`results.txt`
