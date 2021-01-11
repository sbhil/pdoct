# PDOCT README

## PURPOSE

## USAGE

### SETTING UP YOUR OCTAVE PATH

### USING PDOCT FROM PURE DATA

## BUILDING

### WINDOWS

### LINUX

## LICENSE

pdoct is licensed under the [GNU GENERAL PUBLIC LICENSE](gnu.org/licenses/gpl-3.0.html). The license can be found online or in LICENSE.txt.

[GNU Octave](gnu.org/software/octave/) is licensed under the [GNU GENERAL PUBLIC LICENSE](gnu.org/licenses/gpl-3.0.html).

[Pure Data](msp.ucsd.edu/software.html) is licensed under the [Standard Improved BSD License](github.com/pure-data/pure-data/blob/master/LICENSE.txt) 

## TODO

* make it possible to have zero or more signal inlets ( make the external handle signals without using CLASS_MAINSIGNALIN)
* make it unneccesary to use the 'param' symbol as the first symbol for the parameter inlet
* decide on a clear way of deciding what is in pdoct.c as oppsoed to ocwrapper.c (integrate them more tighly or separate completely....)
* create pd help file
* expand pd example patch
* further develop octave example .m-file
* make octave read the sytstem .octaverc files, not just the local users
* add some error handling to e.g.
 * invalid filenames
 * same number of in/out for octave functions and pdoct external
 