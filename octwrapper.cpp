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

#include <oct.h>
#include <octave.h>
#include <parse.h>
#include <interpreter.h>

#include "octwrapper.h"

void octwrapper_init()
{
    static octave::interpreter interpreter;
   
    interpreter.initialize_history(false);
    interpreter.initialize_load_path(true);
    interpreter.read_site_files(true);
    interpreter.read_init_files(true); //.octaverc from current folder and local users home folder
    interpreter.execute();
}

static octave_value_list octwrapper_parse(const char* args)
{
    octave_value_list args2;

    return args2;
}


void octwrapper_run(const char* funcname, float** input, unsigned int ninput, float** output, unsigned int noutput, unsigned int nsamples, float param)
{
    //copy input values from input buffers
    FloatNDArray nd(dim_vector(nsamples, ninput));
    for (size_t i = 0; i < ninput; i++)
        memcpy(nd.column(i).fortran_vec(), input[i], sizeof(float)*nsamples);
    
    //create octave inputs
    octave_value_list in;
    in(0) = octave_value(nd);
    in(1) = octave_value(param);

    //run octave script
    octave_value_list out = octave::feval(funcname, in, 1);
    
    //copy output values to output buffer
    for (size_t i = 0; i < noutput; i++)
        memcpy(output[i], out(0).float_array_value().column(i).fortran_vec(), sizeof(float)*nsamples);   
}