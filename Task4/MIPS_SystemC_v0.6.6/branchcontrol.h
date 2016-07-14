#ifndef BRANCHCONTROLMOD_H
#define BRANCHCONTROLMOD_H

/**
 *
 * BRANCONTROL module interface.
 */

#include <systemc.h>

/**
 * BRANCHCONTROL module.
 * BRANCHCONTROL module models the comparation of the MIPS BNE and BEQ.
 *   - input ports
 *   	- \c sc_uint<32> \c din1	- first operand
 *   	- \c sc_uint<32> \c din2	- second operand
 *   	- \c sc_uint<2>  \c sel		- selects type of branch
 *   - output ports
 *   	- \c bool        \c dout	- active if result = 0 (BEQ) or result != 0 (BNE)
 */

SC_MODULE(branchcontrol) {
  
  sc_in< sc_uint<32> >  din1;
  sc_in< sc_uint<32> >  din2;        
  sc_in< bool >  sel;        
  sc_out< bool > dout;

  SC_CTOR(branchcontrol)
     {      
      SC_METHOD(calc);
      sensitive << din1 << din2 << sel;
    }
  
  void calc();
};

#endif
