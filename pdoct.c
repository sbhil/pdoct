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

  //number of inputs
  t_float f_nin;

  //number of outputs
  t_float f_nout;

  //needed for replacing float inputs at the signal inlet
  t_float f_dummy;

  //octave script name
  t_symbol *s_octfunc;

  //parameter list
  t_float f_param;

  //parameter list inlet
  t_inlet* x_param; //TODO: change this to a list of parameters (char array?)

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
  post("pdoct_perform: start");

  //get passed data
  t_pdoct    *x          = (t_pdoct *)   (w[1]);
  t_sample  **in         = (t_sample **) (w[2]);
  t_sample  **out        = (t_sample **) (w[3]);
  int         nsamples   = (int)         (w[4]);

  //do processing
  oct_wrapper_run(x->s_octfunc->s_name, in, x->f_nin, out, x->f_nout, nsamples, x->f_param);

  post("pdoct_perform: end");

  return (w+5); 
}

void pdoct_dsp(t_pdoct *x, t_signal **sp)
{
  post("pdoct_dsp: start");

  //signals are numbered with inputs first followed by outputs, both ordered from left to right...

  //input pointers
  for (size_t i = 0; i < x->f_nin; i++)
    x->x_f_in[i] = sp[i]->s_vec;

  //output pointers
  for (size_t i = 0; i < x->f_nout; i++) 
    x->x_f_out[i] = sp[(int)x->f_nin+i]->s_vec;

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
  //new instance
  t_pdoct *x = (t_pdoct *)pd_new(pdoct_class);

  //parse constructor arguments
  if (argc == 0)
  {
    //error
  }
  if (argc > 0)
  {
    x->s_octfunc = atom_getsymbol(argv);
    x->f_nin = x->f_nout = 1;
  }
  if (argc > 1)
  {
    x->f_nin = x->f_nout = atom_getfloat(argv+1);
  }
  if (argc > 2)
  {
    x->f_nout = atom_getfloat(argv+2);
  }
  if (argc > 3)
  {
    //don't care?
  }

  //init params
  x->f_param = 0;

  //store script name
  x->s_octfunc = s;

  //create inlet for parameter argument list
  x->x_param = floatinlet_new(&x->x_obj, &x->f_param); 

  //create signal inlets
  x->x_sig_in = malloc(x->f_nin*sizeof(t_inlet*)); //...allocate memory for inlets
  for (size_t i = 0; i < x->f_nin-1; i++)
    x->x_sig_in[i] = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);

  //create signal outlets
  x->x_sig_out = malloc(x->f_nout*sizeof(t_outlet*)); //...allocate memory for outlets
  for (size_t i = 0; i < x->f_nout; i++)  
    x->x_sig_out[i] = outlet_new(&x->x_obj, &s_signal);

  //allocate data buffer pointers
  x->x_f_in = malloc(x->f_nin*sizeof(t_sample*));
  x->x_f_out = malloc(x->f_nout*sizeof(t_sample*));

  //initialize the octave wrapper
  oct_wrapper_init();

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
  //deallocate inlet and outlet memory
  free(x->x_sig_in);
  free(x->x_sig_out); 

  free(x->x_f_in);
  free(x->x_f_out); 
  
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
    f_dummy
  );

}