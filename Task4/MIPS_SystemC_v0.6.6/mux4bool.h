#ifndef MUX4BOOLMOD_H
#define MUX4BOOLMOD_H

#include <systemc.h>

template <class T> class mux4bool: public sc_module
{
  public:
    sc_in< T >  din0;		// ORIGINAL
    sc_in< T >  din1; 		// MEM1
    sc_in< T >  din2;    	// MEM2
    sc_in< T >  din3;    	// WB
    sc_in< bool >  sel1;  
    sc_in< bool >  sel2;       
    sc_out< T > dout;
  
  SC_CTOR(mux4bool)
  {      
    SC_METHOD(entry);
    sensitive << din0 << din1 << din2 << din3 << sel1 << sel2;
  }
  
  void entry();
};

template <class T> void mux4bool<T>::entry()
{
  if(sel1 == 0 && sel2 == 0)		// 00 - original
  {
	dout.write(din0.read());
  }
  else if(sel1 == 0 && sel2 == 1)	// 01 - MEM1
  {
	dout.write(din1.read());
  }
  else if(sel1 == 1 && sel2 == 0)	// 10 - MEM2
  {
	dout.write(din2.read());
  }
  else if(sel1 == 1 && sel2 == 1)	// 11 - WB
  {
	dout.write(din3.read());
  }
  
  // se o mux estiver na fase MEM, ent o 01 é do mem2, o 10 é do WB e o 11 é do WBAUX
}

#endif