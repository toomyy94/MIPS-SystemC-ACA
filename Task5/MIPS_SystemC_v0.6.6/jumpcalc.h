#ifndef JUMPCALCMOD_H
#define JUMPCALCMOD_H

/**
 *
 * \c jumpcalc module interface.
 */

#include <systemc.h>

/**
 * \c jumpcalc module.
 * \c jumpcalc module calculates next PC address for jr and jalr
 *   - input ports
 *   	- \c sc_uint<26> \c din0	- jlabel
 *   	- \c sc_uint<32>  \c din1	- PC4
 *   - output ports
 *   	- \c sc_uint<32> \c dout	- output
 */

SC_MODULE(jumpcalc) {
  
  sc_in < sc_uint<26> >  din0;
  sc_in < sc_uint<32> >  din1;
  sc_out< sc_uint<32> >  dout;

  SC_CTOR(jumpcalc)
     {      
      SC_METHOD(calc);
      sensitive << din0 << din1;
    }
  
  void calc();
};

#endif
