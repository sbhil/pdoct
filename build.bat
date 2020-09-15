clear

C:/cygwin64/bin/x86_64-w64-mingw32-g++.exe ^
-I C:\Octave\octave-5.2.0\mingw64\include\octave-5.2.0\octave ^
-c c:\Users\sbh\Dropbox\Synth\pd\externals\pdoct\octwrapper.cpp

C:/cygwin64/bin/x86_64-w64-mingw32-gcc.exe ^
-I "C:/Program Files/Pd/src/" ^
-c pdoct.c

C:/cygwin64/bin/x86_64-w64-mingw32-g++.exe ^
-shared ^
-I "C:/Program Files/Pd/src/" "C:/Program Files/Pd/bin/pd.dll" ^
-L C:\Octave\octave-5.2.0\mingw64\bin ^
-l liboctave-7 ^
-l liboctinterp-7 ^
-o pdoct.dll pdoct.o octwrapper.o 

rm pdoct.o
rm octwrapper.o