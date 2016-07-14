
#include "jumpcalc.h"

/**
 * \c jumpcalc module callback function.
 */
void jumpcalc::calc()
{
    sc_int<26> aux26;  
    sc_int<32> aux32;
    sc_uint<32> auxu;
    
    aux26=din0.read();
    aux32=aux26;
    auxu=aux32;
    dout.write((din1.read() & 0xF0000000) | ((auxu << 2) & 0x0FFFFFFF));
}
