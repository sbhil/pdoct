/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           *
 *  pdoct is a Pure Data external for processing audio using GNU Octave      *
 *  scripts.                                                                 *
 *                                                                           *
 *  Copyright (C) 2020 Sebastian Boli Hildebrandt.                           *
 *                                                                           *
 *  This program is free software: you can redistribute it and/or modify     *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation, either version 3 of the License, or        *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.   *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <interpreter.h>
#include <oct.h>
#include <octave.h>
#include <parse.h>

#include "octwrapper.h"


char octwrapper_msg[250];

bool octwrapper_init()
{
    static octave::interpreter interpreter;
   
    interpreter.initialize_history(false);
    interpreter.initialize_load_path(true);
    interpreter.read_site_files(true);
    interpreter.read_init_files(true); //.octaverc from current folder and local users home folder
    interpreter.execute();
    
    return true;
}

static octave_value_list octwrapper_argparse(const char* newargs)
{
    static std::string args;
    
    octave_value_list octargs;

    if(args.compare(newargs))
    {
        args = newargs;

        //parse string



    }  

    return octargs;
}


bool octwrapper_run(const char* funcname, float** input, unsigned ninput, float** output, unsigned noutput, unsigned nsamples, float param)
{
    //copy input values from pd input buffers to octave data type
    FloatNDArray nd(dim_vector(nsamples, ninput));
    for (size_t i = 0; i < ninput; i++)    
        for (size_t j = 0; j < nsamples; j++)
            nd.elem(j,i) = input[i][j];

    //create octave input
    octave_value_list in;
    in(0) = octave_value(nd);
    in(1) = octave_value(param);

    //run octave script
    octave_value_list out = octave::feval(funcname, in, 1);
    
    //copy output values from octave data type to pd output buffer
    for (size_t i = 0; i < noutput; i++)
        for (size_t j = 0; j < nsamples; j++)
            output[i][j] = out(0).float_array_value().elem(j,i);

    //print error or debug msg
    //sprintf(octwrapper_msg, "param: %f", param);

    return true; //TODO: add return codes
}