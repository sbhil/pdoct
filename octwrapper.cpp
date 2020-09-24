/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           *
 *  pdoct is an external for Pure Data for processing data using GNU Octave  *
 *  scripts.                                                                 *
 *                                                                           *
 *  Copyright (C) 2020 Sebastian Boli Hildebrandt                            *
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

//remove these....
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <defaults.h>

void oct_wrapper_init()
{
    static octave::interpreter interpreter;
   
    interpreter.initialize_history(false);
    //interpreter.get_load_path().append(octave::config::site_defaults_file()); //current versions .octvaerc
    //interpreter.get_load_path().append(octave::config::startupfile_dir()); //current versions .octvaerc
    //interpreter.get_load_path().append(octave::config::local_site_defaults_file()); //all versions .octaverc
    //interpreter.get_load_path().append(octave::config:: local_startupfile_dir()); //all versions .octaverc
    //interpreter.get_load_path().update();
    //interpreter.get_load_path().set();
    interpreter.initialize_load_path(true);
    interpreter.read_site_files(true);
    interpreter.read_init_files(true); //.octaverc deom current folder and local users home folder
    
    //interpreter.initialize();
    interpreter.execute();

#if 1
  std::ofstream log;
  log.open ("log.txt");
  log << "interpreter.get_load_path().path(): " << interpreter.get_load_path().path() << std::endl;
  log << "interpreter.get_load_path().system_path(): " << interpreter.get_load_path().system_path() << std::endl;
  log << "interpreter.get_load_path().get_command_line_path(): " << interpreter.get_load_path().get_command_line_path()  << std::endl;
  //log << "interpreter.get_environment().exec_path(): " << interpreter.get_environment().exec_path() << std::endl;
  //log << "octave::config::local_fcn_file_dir(): " << octave::config::local_fcn_file_dir() << std::endl;
  //log << "octave::config::local_oct_file_dir(): " << octave::config::local_oct_file_dir() << std::endl;
  log << "octave::config::local_site_defaults_file(): " << octave::config::local_site_defaults_file() << std::endl;
  log << "octave::config::local_startupfile_dir(): " << octave::config::local_startupfile_dir() << std::endl;
  //log << "octave::config::oct_file_dir(): " << octave::config::oct_file_dir() << std::endl;
  //log << "octave::config::octave_exec_home(): " << octave::config::octave_exec_home() << std::endl;
  //log << "octave::config::octave_home(): " << octave::config::octave_home() << std::endl;
  log << "octave::config::site_defaults_file(): " << octave::config::site_defaults_file() << std::endl;
  log << "octave::config::startupfile_dir(): " << octave::config::startupfile_dir() << std::endl;
  log.close();
  #endif

}

static octave_value_list oct_wrapper_parse_arguments(const char* args)
{
    octave_value_list args2;

    return args2;
}


void oct_wrapper_run(const char* funcname, float** input, unsigned int ninput, float** output, unsigned int noutput, unsigned int nsamples, float param)
{
    //FloatNDArray nd;
    //FloatMatrix m;
    FloatRowVector f(nsamples);
    memcpy(f.fortran_vec(), input[0], sizeof(float) * nsamples);
    
    //input values
    octave_value_list in;
    in(0) = octave_value(f);
    in(1) = octave_value(param);

    octave_value_list out = octave::feval(funcname, in, 1);

    memcpy(output[0], out(0)./*float_array_value()*/float_row_vector_value().fortran_vec(), sizeof(float) * nsamples);   
}


#if 0
void octwrapper(const char* funcname, float* input, float* output, int n, float param)
{
    // Create interpreter
    static octave::interpreter interpreter;
        
    try
    { 
        // Inhibit reading history file by calling
        //
        interpreter.initialize_history (false);

        // Set custom load path here if you wish by calling
        //
        interpreter.initialize_load_path (false);

        // Perform final initialization of interpreter, including
        // executing commands from startup files by calling
        //
        //   int status interpreter.initialize ();
        //
        //   if (! interpreter.initialized ())
        //     {
        //       std::cerr << "Octave interpreter initialization failed!"
        //                 << std::endl;
        //       exit (status);
        //     }
        //
        // You may skip this step if you don't need to do anything
        // between reading the startup files and telling the interpreter
        // that you are ready to execute commands.

        // Tell the interpreter that we're ready to execute commands

        if (interpreter.execute() != 0)
        {
            std::cerr << "creating embedded Octave interpreter failed!" << std::endl;
        }

        // Copy input buffer to argument list
        FloatRowVector f(n);
        memcpy(f.fortran_vec(), input, sizeof(float) * n);
        octave_value_list in;
        in(0) = octave_value(f);

        //Place other arguments in list
        in(1) = octave_value(param);
 
        // Call function
        octave_value_list out = octave::feval(funcname, in, 1);

        // Copy output to output buffer
        memcpy(output, out(0).float_row_vector_value().fortran_vec(), sizeof(float) * n);
       
    }
    catch (const octave::exit_exception &ex)
    {
        std::cerr << "Octave interpreter exited with status = " << ex.exit_status() << std::endl;
    }
    catch (const octave::execution_exception &)
    {
        std::cerr << "error encountered in Octave evaluator!" << std::endl;
    }
}
#endif