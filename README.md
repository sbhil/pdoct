# PDOCT README

## PURPOSE

## USAGE

### SETTING UP YOUR OCTAVE PATH

### USING PDOCT FROM PURE DATA

## BUILDING

### WINDOWS

### LINUX

## LICENSE

## TODO

* create pd external with variable number of input/outs (dsp_addv for dsp processing and ? for adding inlets/outlets)
* solve issue where additional input wont let signals connct to them
* make parser for a list of parameters to pass from pd to octave
* create pd patch that concatenates several parameters (floats or strings) into one list to pass to pdoct~
* create pd help file
* make octave read the sytstem .octaverc files, not just the local users
* add some error handling to e.g.
 * launching the octave interpreter
 * invalid filenames
 * same number of in/out for octave functions and pdoct external
 