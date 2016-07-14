//mips.h

#ifndef MIPSMOD_H
#define MIPSMOD_H

/**
 *
 * MIPS module interface.
 */

#include <systemc.h>

#include "imem.h"
#include "decode.h"
#include "regfile.h"
#include "alu.h"
//#include "branchcontrol.h" //branchs
#include "jumpcontrol.h" 
#include "dmem.h"
#include "control.h"
#include "hazard.h"

#include "mux.h"
#include "mux4.h"

#include "reg.h"
#include "ext.h"
#include "shiftl2.h"
#include "add.h"
#include "gates.h"
//#include "comp.h"
#include "jumpcalc.h"

#include "regT.h"
#include "reg_if_id1.h"
#include "reg_id1_id2.h"
#include "reg_id2_exe.h"
#include "reg_exe_mem.h"
#include "reg_mem_wb.h"

/**
 * MIPS module.
 * MIPS module is the main module of the MIPS simulator.
 * Instruction memory, register file, ALU, data memory, etc are instatiated
 * %and interconnected inside this module.
 *
 *   - input ports
 *   	- \c bool \c reset	- reset
 *   	- \c bool \c clk	- clock
 *
 * \image latex MIPS_datapathPipe3.eps "architecture of mips" width=15cm
 */

SC_MODULE(mips) {

   // Ports
   sc_in < bool > clk;
   sc_in < bool > reset;

   // Modules
   // IF 
   registo           *PCreg;     // PC register
   imem              *instmem;   // instruction memory
   add *add4;                    // adds 4 to PC
   mux4< sc_uint<32> > *mPC;      // selects Next PC from PCbrach and PC + 4

   //ID1
   decode            *dec1;      // decodes instruction
   regfile           *rfile;     // register file
   orgate *or_reset_id2_exe, *or_reset_if_id; // -> add
   hazard *hazard_unit;

   //ID2
   control           *ctrl;      // control
   jumpcontrol     *jctrl1;    // ---> add Branch Control
   mux< sc_uint<5> >  *mr, *mr2;       // selects destination register
   ext *e1;                      // sign extends imm to 32 bits

   //andgate *a1;                  // beq instruction and equal values
   //comp *c1;                     //comparador
   shiftl2 *sl2;                 // shift left 2 imm_ext
   add *addbr;                   // adds imm to PC + 4
   //shiftSum *sft;

   //EXE
   alu               *alu1;      // ALU
   mux< sc_uint<32> > *m1, *muxrs;       // selects 2nd ALU operand
   
   //MEM
   dmem              *datamem;   // data memory

   //WB
   mux< sc_uint<32> > *m2;       // selects value to write in register (ALUout or MemOut)

   //pipeline registers
   reg_if_id1_t       *reg_if_id1;
   reg_id1_id2_t      *reg_id1_id2;
   reg_id2_exe_t      *reg_id2_exe;
   reg_exe_mem_t      *reg_exe_mem;
   reg_mem_wb_t       *reg_mem_wb;
   jumpcalc           *jcalc;

   // Signals

   // IF
   sc_signal < sc_uint<32> > PC,       // Program Counter
                             NPC,      // Next Program Counter
			     PC4;      // PC + 4
   sc_signal < sc_uint<32> > inst;     // current instruction

   
   sc_signal <bool> enable_pc, enable_regfile, enable_ifid1, enable_id1id2, enable_id2exe; //adicionado


   //ID
   sc_signal < sc_uint<32> > inst_id1,  // current instruction ID phase
                             PC4_id1;
   // instruction fields
   sc_signal < sc_uint<5> > rs, rt, rd;
   sc_signal < sc_uint<16> > imm;
   sc_signal < sc_uint<6> > opcode;
   sc_signal < sc_uint<5> > shamt;
   sc_signal < sc_uint<6> > funct;
   // register file signals
   sc_signal < sc_uint<5> > WriteReg;  // register to write

   sc_signal < sc_uint<32> > regdata1, // value of register rs
                             regdata2, // value of regiter rt
                             regdata3,
			     WriteVal; // value to write in register WriteReg


   sc_signal < sc_uint<32> > rega_id2, // value of register rs EXE phase
                             regb_id2, // value of regiter rt EXE phase
                             regb_mem; // value of regiter rt MEM phase


   sc_signal <bool> reset_id1id2, reset_haz_id1id2, reset_haz_id2exe, reset_id2exe, reset_ifid1, reset_haz_ifid1 ;


   // the following two signals are not used by the architecture
   // they are used only for visualization purposes
   sc_signal < sc_uint<32> > PC_id1;      // PC of instruction in ID
   sc_signal < bool >        valid_id1;   // true if there is an instruction in ID


   sc_signal < sc_uint<16> > imm_value, imm_id2;  // imm sign extended // deixa de existir



   //ID2

   // control signals
   sc_signal <bool> MemRead, MemWrite, MemtoReg;
   sc_signal <bool> RegWrite, RegDst;
   sc_signal <bool> ALUSrc, ALUSrc_exe;
   sc_signal < sc_uint<3> > ALUOp;
   sc_signal < sc_uint<26> > jlabel, jlabel_id2; 
   sc_signal < sc_uint<5> > ra, ra_id2, WriteRDst; 



   //sc_signal <bool> Branch;// comp_res; //branchs
   sc_signal < sc_uint<3> >  Jump;
   sc_signal <bool> Jreg;



   sc_signal < sc_uint<32> > imm_ext;  // imm sign extended

   sc_signal < sc_uint<32> > PC4_id2;


   sc_signal < sc_uint<32> > PC_id2;      // PC of instruction in ID
   sc_signal < bool >        valid_id2;   // true if there is an instruction in ID


   sc_signal < sc_uint<5> > rt_id2, rd_id2, rs_id2 ;
   sc_signal < sc_uint<6> > opcode_id2, funct_id2;

 




   //EXE
   sc_signal < bool > Zero;            // ALU output is zero
   sc_signal < sc_uint<32> > PC4_exe;
   sc_signal < sc_uint<32> > addr_ext; // imm_ext shift left 2
   sc_signal < sc_uint<5> > WriteReg_exe;
   sc_signal < sc_uint<5> > WriteReg_id2; //addd
   // ALU signals
   sc_signal < sc_uint<32> > ALUIn2,   // ALU second operand
                             ALUOut;   // ALU Output
   sc_signal <bool> MemRead_exe, MemWrite_exe, MemtoReg_exe;
   sc_signal <bool> RegWrite_exe;
   sc_signal < sc_uint<3> > ALUOp_exe;
   sc_signal <bool> Branch_exe;
   
   // the following two signals are not used by the architecture
   // they are used only for visualization purposes
   sc_signal < sc_uint<32> > PC_exe;     // PC of instruction in ID
   sc_signal < bool > valid_exe;         // true if there is an instruction in ID


   sc_signal < sc_uint<32> > regb_exe, rega_exe; // value of regiter rt EXE phase
   sc_signal < sc_uint<32> > imm_exe;  // imm sign extended


   //MEM
   sc_signal < sc_uint<32> > MemOut;   // data memory output
   sc_signal < sc_uint<32> > ALUOut_mem, BranchTarget_mem;   
   sc_signal < sc_uint<5> > WriteReg_mem;   
   sc_signal <bool> MemRead_mem, MemWrite_mem, MemtoReg_mem;
   sc_signal <bool> RegWrite_mem;
   sc_signal <bool> Branch_mem, Zero_mem;

   // the following two signals are not used by the architecture
   // they are used only for visualization purposes
   sc_signal < sc_uint<32> > PC_mem;   
   sc_signal < bool > valid_mem;

   //WB
   sc_signal < sc_uint<32> > MemOut_wb, ALUOut_wb;   
   sc_signal < sc_uint<5> > WriteReg_wb;   
   sc_signal <bool> MemtoReg_wb;
   sc_signal <bool> RegWrite_wb;

   // the following two signals are not used by the architecture
   // they are used only for visualization purposes
   sc_signal < sc_uint<32> > PC_wb;   
   sc_signal < bool > valid_wb;

   //nonpipelined signals
   sc_signal < sc_uint<32> > BranchTarget; // PC if branch
   sc_signal < sc_uint<32> > JumpTarget; // PC if branch
   
   sc_signal < sc_uint<2> > JumpTaken;       // execute branch
   sc_signal < sc_uint<32> > const4;   // contant 4
   sc_signal < bool > const1;          // contant 4

   SC_CTOR(mips) {
       buildArchitecture();
   }

   ~mips(void);

   void buildArchitecture();

   void buildIF();
   void buildID1();
   void buildID2();
   void buildEXE();
   void buildMEM();
   void buildWB();
};

#endif
