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

#include <ov-struct.h>
#include <ov-base.h>
#include <ostream>
#include "pdoct.h"

char octwrapper_msg[1000];

bool octwrapper_init()
{
    static octave::interpreter interpreter;

    interpreter.initialize_history(false);
    interpreter.initialize_load_path(true);
    interpreter.read_site_files(true);
    interpreter.read_init_files(true); //.octaverc from current folder and local users home folder
    bool success = interpreter.execute();

    return success;
}

bool octwrapper_run(const char *funcname, float **input, unsigned ninput, float **output, unsigned noutput, unsigned nsamples, unsigned nparam, const char *paramid, float *paramval)
{

    //redirect Octave's stdout to pd
    std::stringstream ss;
    ss << octave_stdout.rdbuf() << std::endl;
    pdoct_post(ss.str().c_str());
    //octave::flush_stdout();
    //ss.flush();

    try
    {
        //copy input values from pd input buffers to octave data type
        FloatNDArray nd(dim_vector(nsamples, ninput));
        for (size_t i = 0; i < ninput; i++)
            for (size_t j = 0; j < nsamples; j++)
                nd.elem(j, i) = input[i][j];

        //create octave input
        octave_value_list in;
        in(0) = octave_value(nd);

        if (nparam > 0)
        {
            octave_struct param;

            //store paramid as 'param.id' field
            param.assign("id", octave_value(paramid));

            //store relevant part of paramval array as 'param.val'
            FloatRowVector val(nparam);

            for (size_t i = 0; i < nparam; i++)
                val(i) = paramval[i];

            param.assign("val", octave_value(val));

            //add to input list
            //in(1) = param.map(umap_unknown);
            //in(1) = param.squeeze();
            in(1) = octave_value(param.map_value());
        }

        //run octave script
        octave_value_list out = octave::feval(funcname, in, 1);

        //copy output values from octave data type to pd output buffer
        for (size_t i = 0; i < noutput; i++)
            for (size_t j = 0; j < nsamples; j++)
                output[i][j] = out(0).float_array_value().elem(j, i);
    }
    catch (const octave::exit_exception &ex)
    {
        sprintf(octwrapper_msg, "octave::exit_exception: Octave interpreter exited with status: %i", ex.exit_status());

        return false;
    }
    catch (const octave::execution_exception &ex)
    {
        sprintf(octwrapper_msg, "octave::execution_exception: error encountered in Octave evaluator: %s", ex.info().c_str());

        return false;
    }

    return true;
}