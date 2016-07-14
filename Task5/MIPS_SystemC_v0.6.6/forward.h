#ifndef FORWARDMOD_H
#define FORWARDMOD_H

#include <systemc.h>

SC_MODULE(forward) {
  
  // Registos RS/RT para comparar
  sc_in< sc_uint<5> > rs_id;
  sc_in< sc_uint<5> > rt_id;
  sc_in< sc_uint<5> > rs_exe;
  sc_in< sc_uint<5> > rt_exe;
  sc_in< sc_uint<5> > rt_mem;
  
  // Registos RD para comparar
  sc_in< sc_uint<5> > WriteReg_mem;
  //sc_in< sc_uint<5> > WriteReg_mem2;
  sc_in< sc_uint<5> > WriteReg_wb;
 // sc_in< sc_uint<5> > WriteReg_aux;
  
  // Para saber se a instrução está a escrever na memoria ou n (EX: SW n escreve)
  sc_in< bool > RegWrite_mem;
  //sc_in< bool > RegWrite_mem2;
  sc_in< bool > RegWrite_wb;
 // sc_in< bool > RegWrite_aux;
  
  // Para saber qd é um SW
  sc_in< bool > MemWrite;
  sc_in< bool > MemRead_wb;
  
  // Sinais de controlo, 2 por cada MUX
  sc_out< bool > rtId1;
  sc_out< bool > rsId1;

  sc_out< bool > rtId2;
  sc_out< bool > rsId2;

  sc_out< bool > rtExe1;
  sc_out< bool > rsExe1;

  sc_out< bool > rtExe2;
  sc_out< bool > rsExe2;
  
  //sc_out< bool > rtMem;
  sc_out< bool > selMem;
  //sc_out< bool > rtMem2;
  
    
  /*
      << WriteReg_mem2   << RegWrite_mem2 
  */

  SC_CTOR(forward)
     {     
	SC_METHOD(calc);
      sensitive << rs_id << rt_id << rs_exe << rt_exe << WriteReg_mem  << WriteReg_wb << RegWrite_mem << RegWrite_wb << MemWrite << MemRead_wb;;
    }
  
  void calc();
};

#endif
