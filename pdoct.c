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

#include <math.h>
#include <stdlib.h>

#include <m_pd.h>

#include "octwrapper.h"



/* ****************************************************************************
 * 
 * DATA SPACE
 *
 * ***************************************************************************/

static t_class *pdoct_class;  

typedef struct _pdoct
{ 
  //this has to be at the top of the list!
  t_object x_obj;

  //number of inputs
  unsigned u_nin;

  //number of outputs
  unsigned u_nout;

  //needed for replacing float inputs at the signal inlets
  t_float* f_dummy;
  t_float  f_dummy1; //this is for the first (main) inlet

  //octave script name
  t_symbol *s_octfunc;

  //parameter list
  t_symbol  *s_params;

  //parameter list inlet
  t_inlet* x_param_in;

  //signal inlets
  t_inlet** x_sig_in;

  //signal outlets
  t_outlet** x_sig_out;

  //input data buffers
  t_sample** x_f_in;

  //ouput data buffers
  t_sample** x_f_out;

} t_pdoct;  

/* ****************************************************************************
 * 
 * METHOD SPACE
 *
 * ***************************************************************************/

t_int *pdoct_perform(t_int *w)
{
  //post("pdoct_perform: start");

  //get passed data
  t_pdoct    *x          = (t_pdoct *)   (w[1]);
  t_sample  **in         = (t_sample **) (w[2]);
  t_sample  **out        = (t_sample **) (w[3]);
  unsigned    nsamples   = (unsigned)    (w[4]);

  //do processing
  bool success = octwrapper_run(x->s_octfunc->s_name, in, x->u_nin, out, x->u_nout, nsamples, "" /*x->s_params->s_name*/);

  post(x->s_params->s_name);

  //check if there is an error message from the octwrapper
  if (!success)
  {
    //post message
    post(octwrapper_msg);
    //clear message
    sprintf(octwrapper_msg, "");
  } 

  //post("pdoct_perform: end");

  return &w[5]; 
}

void pdoct_dsp(t_pdoct *x, t_signal **sp)
{
  post("pdoct_dsp: start");

  //signals are counted with inputs first followed by outputs, both ordered from left to right...

  //input pointers
  for (size_t i = 0; i < x->u_nin; i++)
    x->x_f_in[i] = sp[i]->s_vec;

  //output pointers
  for (size_t i = 0; i < x->u_nout; i++)
    x->x_f_out[i] = sp[i+x->u_nin]->s_vec;

  dsp_add
  (
    // add perform rutine to dsp tree
    pdoct_perform, 
    //number of arguments to pass
    4, 
    //actual arguments
    x, //object data
    x->x_f_in, //input pointers
    x->x_f_out, //output pointers
    sp[0]->s_n //vector length (same for all inputs and outputs)
  );

  post("pdoct_dsp: end");
}


/* ****************************************************************************
 * 
 * CONSTRUCTOR
 *
 * ***************************************************************************/ 

void *pdoct_new(t_symbol *s, int argc, t_atom *argv)  
{ 
  post("pdoct_new (constructor): start");

  //new instance
  t_pdoct *x = (t_pdoct *)pd_new(pdoct_class);

  //parse constructor arguments
  if (argc == 0)
  {
    //error or passthrough?
  }
  if (argc > 0)
  {
    x->s_octfunc = atom_getsymbol(argv);
    x->u_nin = x->u_nout = 1;
  }
  if (argc > 1)
  {
    x->u_nin = fmaxf(atom_getfloat(argv+1), 1); //TODO: let zero inlets be allowed
    x->u_nout = fmaxf(atom_getfloat(argv+1), 1); //always at least one outlet
  }
  if (argc > 2)
  {
    x->u_nout = fmaxf(atom_getfloat(argv+2), 1); //always at least one outlet
  }
  if (argc > 3)
  {
    //don't care?
  }

  //create signal inlets
  x->f_dummy = malloc((x->u_nin-1)*sizeof(t_float));
  x->x_sig_in = malloc((x->u_nin-1)*sizeof(t_inlet*)); //...allocate memory for inlets
  for (size_t i = 0; i < x->u_nin-1; i++)
    //x->x_sig_in[i] = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    x->x_sig_in[i] = signalinlet_new(&x->x_obj, x->f_dummy[i]);

  //create inlet for parameter input
  //x->x_param_in = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
  x->s_params = malloc(sizeof(t_symbol));
  x->x_param_in = symbolinlet_new(&x->x_obj, &x->s_params);

  //create signal outlets
  x->x_sig_out = malloc(x->u_nout*sizeof(t_outlet*)); //...allocate memory for outlets
  for (size_t i = 0; i < x->u_nout; i++)  
    x->x_sig_out[i] = outlet_new(&x->x_obj, &s_signal);

  //allocate memory for data buffer pointers
  x->x_f_in  = malloc(x->u_nin*sizeof(t_sample*));
  x->x_f_out = malloc(x->u_nout*sizeof(t_sample*));

  //initialize the octave wrapper
  bool success = octwrapper_init();

   //check if there is an error message from the octwrapper
  if (!success)
  {
    //post message
    post(octwrapper_msg);
    //clear message
    sprintf(octwrapper_msg, "");
  } 

  post("pdoct_new (contructor): end");

  return (void *)x;  
}  

/* ****************************************************************************
 * 
 * DESTRUCTOR
 *
 * ***************************************************************************/ 

void *pdoct_delete(t_pdoct* x)
{
  post("pdoct_delete (destructor): start");

  //deallocate inlet and outlet memory
  inlet_free(x->x_param_in);
  for (size_t i = 0; i < x->u_nin-1; i++)
    inlet_free(x->x_sig_in[i]);
  for (size_t i = 0; i < x->u_nout; i++)
    outlet_free(x->x_sig_out[i]);
  free(x->x_sig_in);
  free(x->x_sig_out); 
  free(x->x_f_in);
  free(x->x_f_out);
  free(x->f_dummy);

  post("pdoct_delete (destructor): end");
}

/* ****************************************************************************
 * 
 * SETUP
 *
 * ***************************************************************************/

void pdoct_setup(void)
{  
  post("pdoct_setup: start");

  pdoct_class = class_new
  (
    //symbolic name of the class
    gensym("pdoct~"), 
    //constructor
    (t_newmethod)pdoct_new,
    //destructor 
    (t_method)pdoct_delete, 
    //static data space allocation
    sizeof(t_pdoct), 
    //graphical representation
    CLASS_DEFAULT,
    //argument list
    A_GIMME,
    //no more arguments ...
    0
  );

  //add dsp method
  class_addmethod
  (
    //class to add method to
    pdoct_class,
    //method
    (t_method)pdoct_dsp,
    //symbolic selctor
    gensym("dsp"),
    //makes it impossible to manually send the dsp message to the object
    A_CANT,
    //no more arguments ...
    0
  );

  //enable signal input at first inlet 
  CLASS_MAINSIGNALIN
  (
    //signal class
    pdoct_class,
    //signal class data space type 
    t_pdoct, 
    //
    f_dummy1
  );

  post("pdoct_setup: end");
}