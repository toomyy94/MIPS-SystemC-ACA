
// branchcontrol.cpp

#include "branchcontrol.h"

/**
 * \c branchcontrol module callback function.
 */
void branchcontrol::calc()
{
    sc_uint<32> a=din1.read();
    sc_uint<32> b=din2.read();

    switch (sel.read())
    {
       case 1: dout.write((a == b) ? 1 : 0);    // beq
               break;
       case 2: dout.write((a != b) ? 1 : 0);    // bne
               break;
       case 3: dout.write(2);  // jump
               break;
       case 4: dout.write(3);  // jr
               break;
       default: dout.write(0);    // other
               break;
    }
}
