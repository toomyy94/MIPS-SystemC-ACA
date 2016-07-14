#ifndef JUMPCONTROLMOD_H
#define JUMPCONTROLMOD_H

/**
 *
 * JUMPCONTROL module interface.
 */

#include <systemc.h>

/**
 * JUMPCONTROL module.
 * JUMPCONTROL module models the comparation of the MIPS BNE and BEQ.
 *   - input ports
 *   	- \c sc_uint<32> \c din1	- first operand
 *   	- \c sc_uint<32> \c din2	- second operand
 *   	- \c sc_uint<2>  \c sel		- selects type of branch
 *   - output ports
 *   	- \c sc_uint<2>  \c dout	- 0 if no jump, 1 if branch, 2 if jump or jal, 3 if jr or jalr
 */

SC_MODULE(jumpcontrol) {
  
  sc_in< sc_uint<32> >  din1;
  sc_in< sc_uint<32> >  din2;        
  sc_in< sc_uint<3> >  sel;        
  sc_out< sc_uint<2> > dout;

  SC_CTOR(jumpcontrol)
     {      
      SC_METHOD(calc);
      sensitive << din1 << din2 << sel;
    }
  
  void calc();
};

#endif
