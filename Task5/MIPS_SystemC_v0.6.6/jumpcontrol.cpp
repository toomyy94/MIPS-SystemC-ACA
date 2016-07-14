
// jumpcontrol.cpp

#include "jumpcontrol.h"

/**
 * \c jumpcontrol module callback function.
 */
void jumpcontrol::calc()
{
    sc_uint<32> a=din1.read();
    sc_uint<32> b=din2.read();

    sc_int<32> asng;
    sc_int<32> bsng;

    asng =a;
    bsng=b; 
     
    switch (sel.read())
    {
       case 1: 
       printf("a=%X\n", (unsigned int)din1.read());
       printf("b=%X\n", (unsigned int)din2.read());

          //if(a == b)
          //    dout.write(1);
          //else
          //    dout.write(0);

               dout.write((asng == bsng) ? 1 : 0 );    // beq
               break;
       case 2: dout.write((asng != bsng) ? 1 : 0) ;    // bne
               break;
       case 3: dout.write(2);  // jump
               break;
       case 4: dout.write(3);  // jr
               break;
       case 5: dout.write((asng > 0) ? 1 : 0);    // bgtz
               break;
       case 6: 
         //       printf("a=%X\n", (unsigned int)din1.read());
           //     printf("b=%X\n", (unsigned int)din2.read());

               dout.write((asng <= 0) ? 1 : 0);    // blez
               break;

       // case 5: dout.write(2);  // jal
       //         break;
       // case 6: dout.write(3);  // jalr
       //         break;
       default: dout.write(0);    // other
               break;
    }
}




