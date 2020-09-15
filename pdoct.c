#include <stdlib.h>

#include <m_pd.h>

#include "octwrapper.h"
 

#include <unistd.h>
#include <stdio.h>
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

  /* other data goes here... */

  //needed for replacing float inputs at the signal inlet
  t_float f_dummy;

  //gain value
  t_float f_gain;

  //octave script name
  t_symbol *s_octfunc;

  //inlets  
  t_inlet *x_in_gain;

  //outlets
  t_outlet *x_out;

} t_pdoct;  

/* ****************************************************************************
 * 
 * METHOD SPACE
 *
 * ***************************************************************************/

t_int *pdoct_perform(t_int *w)
{
  //get passed data
  t_pdoct   *x   = (t_pdoct *) (w[1]);
  t_sample  *in  = (t_sample *)(w[2]);
  t_sample  *out = (t_sample *)(w[3]);
  int        n   = (int)       (w[4]);

  //get parameters from object data
  t_sample f_gain = x->f_gain;

  //do processing
  oct_wrapper_run(x->s_octfunc->s_name, in, out, n, f_gain);

  return (w+5);
}

void pdoct_dsp(t_pdoct *x, t_signal **sp)
{
  dsp_add
  (
    // add perform rutine to dsp tree
    pdoct_perform, 
    //number of arguments to pass
    4, 
    //actual arguments
    x,            //object data
    sp[0]->s_vec, //input vector
    sp[1]->s_vec, //output vector
    sp[0]->s_n    //vector length (same for input and output)
  );
}


/* ****************************************************************************
 * 
 * CONSTRUCTOR
 *
 * ***************************************************************************/ 

void *pdoct_new(t_symbol *s)  
{  
  //new instance
  t_pdoct *x = (t_pdoct *)pd_new(pdoct_class);

  //store gain value
  x->f_gain = 1;

  //store script name
  x->s_octfunc = s;

  //create gain inlet
  x->x_in_gain = floatinlet_new (&x->x_obj, &x->f_gain); 

  //create signal outlet
  x->x_out = outlet_new(&x->x_obj, &s_signal);

  //initialize the octave wrapper
  oct_wrapper_init();

  return (void *)x;  
}  

/* ****************************************************************************
 * 
 * DESTRUCTOR
 *
 * ***************************************************************************/ 

void *pdoct_delete(void)
{

}

/* ****************************************************************************
 * 
 * SETUP
 *
 * ***************************************************************************/

void pdoct_setup(void)
{  

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
    //first argument: script name
    A_DEFSYMBOL,
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
    f_dummy
  );

}