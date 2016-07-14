//mips.cpp

/**
 *
 * MIPS module implementation.
 */

#include "mips.h"

/**
 * buils IF stage components 
 */
void mips::buildIF(void)
{
      // Program Counter
      PCreg = new registo ("PCregister");
      PCreg->din(NPC);
      PCreg->dout(PC);
      PCreg->clk(clk);
      PCreg->reset(reset);
      PCreg->enable(enable_pc);

      // Instruction Memory
      instmem = new imem ("imem");
      instmem->addr(PC);
      instmem->inst(inst);

      // Adds 4 to Program Counter
      add4 = new add ("add4"); 

      add4->op1(PC);
      add4->op2(const4);
      add4->res(PC4);

      // Selects Next Program Counter Value
      mPC = new mux< sc_uint<32> > ("mPC");
   
      mPC->sel(BranchTaken);
      mPC->din0(PC4);
      mPC->din1(BranchTarget_mem);
      mPC->dout(NPC);
}

/**
 * buils ID stage components 
 */
void mips::buildID1(void)
{
      // Decodes Instruction
      dec1 = new decode ("decode");
      dec1->inst(inst_id1);
      dec1->rs(rs);
      dec1->rt(rt);
      dec1->rd(rd);
      dec1->imm(imm);
      dec1->opcode(opcode);
      dec1->shamt(shamt);
      dec1->funct(funct);

   
      // Register File
      rfile = new regfile ("regfile");

      rfile->reg1( rs );
      rfile->reg2( rt );
      rfile->regwrite(WriteReg_wb);

      rfile->wr(RegWrite_wb);
      rfile->datawr(WriteVal);

      rfile->clk(clk);
      rfile->reset(reset);
      rfile->enable(enable_regfile);
}


/**
 * buils ID stage components 
 */
void mips::buildID2(void)
{

      rfile->data1( rega_id2 );
      rfile->data2( regb_id2 );

      // Selects Register to Write
      mr = new mux< sc_uint<5> > ("muxRDst");
   
      mr->sel(RegDst);
      mr->din0(rt_id2);
      mr->din1(rd_id2);
      mr->dout(WriteReg);

      // 16 to 32 bit signed Immediate extension
      e1 = new ext("ext");
      e1->din(imm_id2);
      e1->dout(imm_ext);

      // Control
      ctrl = new control ("control");

      ctrl->opcode(opcode_id2); 
      ctrl->funct(funct_id2);   
      ctrl->RegDst(RegDst); 
      ctrl->Branch(Branch);
      ctrl->MemRead(MemRead);
      ctrl->MemWrite(MemWrite);
      ctrl->MemtoReg(MemtoReg);
      ctrl->ALUOp(ALUOp);
      ctrl->ALUSrc(ALUSrc);
      ctrl->RegWrite(RegWrite);
}


/**
 * buils EXE stage components 
 */
void mips::buildEXE(void)
{
      // Selects second operand of ALU
      m1 = new mux< sc_uint<32> > ("muxOp");

      m1->sel(ALUSrc_exe);
      m1->din0(regb_exe);
      m1->din1(imm_exe);
      m1->dout(ALUIn2);

      // ALU
      alu1 = new alu("alu");

      alu1->din1(rega_exe);
      alu1->din2(ALUIn2);
      alu1->op(ALUOp_exe);
      alu1->dout(ALUOut);
      alu1->zero(Zero);

      // shift left 2 imm_ext 
      sl2 = new shiftl2("sl2");
      sl2->din(imm_exe);
      sl2->dout(addr_ext);

      // Adds Branch Immediate to Program Counter + 4
      addbr = new add ("addbr");
   
      addbr->op1(PC4_exe);
      addbr->op2(addr_ext);  
      addbr->res(BranchTarget);

}

/**
 * buils MEM stage components 
 */
void mips::buildMEM(void)
{
      // Data Memory
      datamem = new dmem ("datamem");
   
      datamem->addr(ALUOut_mem);
      datamem->din(regb_mem);
      datamem->dout(MemOut);
      datamem->wr(MemWrite_mem);
      datamem->rd(MemRead_mem);
      datamem->clk(clk);

      // Enables Branch
      a1 = new andgate ("a1");

      a1->din1(Branch_mem);
      a1->din2(Zero_mem);
      a1->dout(BranchTaken);
}

/**
 * buils WB stage components 
 */
void mips::buildWB(void)
{
      // Selects Result
      m2 = new mux< sc_uint<32> > ("muxRes");
   
      m2->sel(MemtoReg_wb);
      m2->din0(ALUOut_wb);
      m2->din1(MemOut_wb);
      m2->dout(WriteVal);

}

/**
 * Instantiates the pipeline registers and calls other functions to
 * buils stage specific components 
 */
void mips::buildArchitecture(void){

      const4 = 4;
      const1 = true;

      buildIF();

      //reg_if_id1
      reg_if_id1 = new reg_if_id1_t("reg_if_id1");
      reg_if_id1->inst_if(inst);
      reg_if_id1->inst_id1(inst_id1);
      reg_if_id1->PC4_if(PC4);
      reg_if_id1->PC4_id1(PC4_id1);
      reg_if_id1->PC_if(PC);
      reg_if_id1->PC_id1(PC_id1);
      reg_if_id1->valid_if(const1);
      reg_if_id1->valid_id1(valid_id1);
      reg_if_id1->clk(clk);
      reg_if_id1->reset(reset_ifid1);
      reg_if_id1->enable(enable_ifid1);
 
      or_reset_if_id = new orgate("or_reset_if_id");
      or_reset_if_id->din1(reset);
      or_reset_if_id->din2(reset_haz_ifid1);
      or_reset_if_id->dout(reset_ifid1);

      buildID1();

      //reg_id1_id2
      reg_id1_id2 = new reg_id1_id2_t("reg_id1_id2");
      
      reg_id1_id2->imm_id1(imm); //imm_ext
      reg_id1_id2->imm_id2(imm_id2); //imm_exe

      reg_id1_id2->rd_id1(rd);
      reg_id1_id2->rd_id2(rd_id2);
      
      reg_id1_id2->rt_id1(rt);
      reg_id1_id2->rt_id2(rt_id2);

	  reg_id1_id2->rs_id1(rs);
      reg_id1_id2->rs_id2(rs_id2);



      reg_id1_id2->opcode_id1(opcode);
      reg_id1_id2->opcode_id2(opcode_id2);  
      reg_id1_id2->funct_id1(funct);
      reg_id1_id2->funct_id2(funct_id2);   


      reg_id1_id2->PC4_id1(PC4_id1);
      reg_id1_id2->PC4_id2(PC4_id2);
      

      reg_id1_id2->PC_id1(PC_id1);
      reg_id1_id2->PC_id2(PC_id2);
      reg_id1_id2->valid_id1(valid_id1);
      reg_id1_id2->valid_id2(valid_id2);
      reg_id1_id2->clk(clk);
      reg_id1_id2->reset(reset);
      reg_id1_id2->enable(enable_id1id2); //add

      //

      buildID2();

      //reg_id_exe
      reg_id2_exe = new reg_id2_exe_t("reg_id2_exe");
      reg_id2_exe->imm_id2(imm_ext);
      reg_id2_exe->imm_exe(imm_exe);
      reg_id2_exe->PC4_id2(PC4_id2);
      reg_id2_exe->PC4_exe(PC4_exe);
      
      reg_id2_exe->WriteReg_id2(WriteReg);
      reg_id2_exe->WriteReg_exe(WriteReg_exe);
      reg_id2_exe->MemRead_id2(MemRead);
      reg_id2_exe->MemRead_exe(MemRead_exe);
      reg_id2_exe->MemWrite_id2(MemWrite);
      reg_id2_exe->MemWrite_exe(MemWrite_exe);
      reg_id2_exe->MemtoReg_id2(MemtoReg);
      reg_id2_exe->MemtoReg_exe(MemtoReg_exe);
      reg_id2_exe->Branch_id2(Branch);
      reg_id2_exe->Branch_exe(Branch_exe);
      reg_id2_exe->RegWrite_id2(RegWrite);
      reg_id2_exe->RegWrite_exe(RegWrite_exe);
      reg_id2_exe->ALUSrc_id2(ALUSrc);
      reg_id2_exe->ALUSrc_exe(ALUSrc_exe);
      reg_id2_exe->ALUOp_id2(ALUOp);
      reg_id2_exe->ALUOp_exe(ALUOp_exe);

      reg_id2_exe->rega_id2(rega_id2); //adicionados
      reg_id2_exe->rega_exe(rega_exe);
      reg_id2_exe->regb_id2(regb_id2);
      reg_id2_exe->regb_exe(regb_exe);
      
      reg_id2_exe->PC_id2(PC_id2);
      reg_id2_exe->PC_exe(PC_exe);
      reg_id2_exe->valid_id2(valid_id2);
      reg_id2_exe->valid_exe(valid_exe);
      reg_id2_exe->clk(clk);
      reg_id2_exe->reset(reset_id2exe);
      reg_id2_exe->enable(const1);

      //
      or_reset_id2_exe = new orgate("or_reset_id2_exe");
      or_reset_id2_exe->din1(reset);
      or_reset_id2_exe->din2(reset_haz_id2exe);
      or_reset_id2_exe->dout(reset_id2exe);


//------------------


      buildEXE();

      //reg_exe_mem
      reg_exe_mem = new reg_exe_mem_t("reg_exe_mem");
      reg_exe_mem->aluOut_exe(ALUOut);
      reg_exe_mem->aluOut_mem(ALUOut_mem);
      reg_exe_mem->MemRead_exe(MemRead_exe);
      reg_exe_mem->MemRead_mem(MemRead_mem);
      reg_exe_mem->MemWrite_exe(MemWrite_exe);
      reg_exe_mem->MemWrite_mem(MemWrite_mem);
      reg_exe_mem->MemtoReg_exe(MemtoReg_exe);
      reg_exe_mem->MemtoReg_mem(MemtoReg_mem);
      reg_exe_mem->Branch_exe(Branch_exe);
      reg_exe_mem->Branch_mem(Branch_mem);
      reg_exe_mem->RegWrite_exe(RegWrite_exe);
      reg_exe_mem->RegWrite_mem(RegWrite_mem);
      reg_exe_mem->Zero_exe(Zero);
      reg_exe_mem->Zero_mem(Zero_mem);
      reg_exe_mem->BranchTarget_exe(BranchTarget);
      reg_exe_mem->BranchTarget_mem(BranchTarget_mem);
      reg_exe_mem->regb_exe(regb_exe);
      reg_exe_mem->regb_mem(regb_mem);
      reg_exe_mem->WriteReg_exe(WriteReg_exe);
      reg_exe_mem->WriteReg_mem(WriteReg_mem);
      reg_exe_mem->PC_exe(PC_exe);
      reg_exe_mem->PC_mem(PC_mem);
      reg_exe_mem->valid_exe(valid_exe);
      reg_exe_mem->valid_mem(valid_mem);
      reg_exe_mem->clk(clk);
      reg_exe_mem->reset(reset);
      reg_exe_mem->enable(const1);

      buildMEM();
      
      //reg_mem_wb
      reg_mem_wb = new reg_mem_wb_t("reg_mem_wb");
      reg_mem_wb->aluOut_mem(ALUOut_mem);
      reg_mem_wb->aluOut_wb(ALUOut_wb);
      reg_mem_wb->memOut_mem(MemOut);
      reg_mem_wb->memOut_wb(MemOut_wb);
      reg_mem_wb->MemtoReg_mem(MemtoReg_mem);
      reg_mem_wb->MemtoReg_wb(MemtoReg_wb);
      reg_mem_wb->RegWrite_mem(RegWrite_mem);
      reg_mem_wb->RegWrite_wb(RegWrite_wb);
      reg_mem_wb->WriteReg_mem(WriteReg_mem);
      reg_mem_wb->WriteReg_wb(WriteReg_wb);
      reg_mem_wb->PC_mem(PC_mem);
      reg_mem_wb->PC_wb(PC_wb);
      reg_mem_wb->valid_mem(valid_mem);
      reg_mem_wb->valid_wb(valid_wb);
      reg_mem_wb->clk(clk);
      reg_mem_wb->reset(reset);
      reg_mem_wb->enable(const1);

      buildWB();

      hazard_unit = new hazard("hazard_unit");
      hazard_unit->rs( rs_id2 );
      hazard_unit->rt( rt_id2 );
      hazard_unit->WriteReg_exe(WriteReg_exe);
      hazard_unit->RegWrite_exe(RegWrite_exe);
      hazard_unit->mem_read(MemRead);
      hazard_unit->RegWrite_mem(RegWrite_mem);
      hazard_unit->WriteReg_mem(WriteReg_mem);
	  hazard_unit->RegWrite_wb(RegWrite_wb); //add
      hazard_unit->WriteReg_wb(WriteReg_wb); //add




      hazard_unit->RegWrite(RegWrite); 
      hazard_unit->BranchTaken(BranchTaken);

      hazard_unit->WriteReg_id2(WriteReg);  

      hazard_unit->enable_pc(enable_pc); //
      hazard_unit->enable_ifid1(enable_ifid1); //
      hazard_unit->enable_id1id2(enable_id1id2);

      hazard_unit->enable_regfile(enable_regfile); //

      hazard_unit->reset_ifid1(reset_haz_ifid1);  //
      hazard_unit->reset_id2exe(reset_haz_id2exe); // 
   }

mips::~mips(void)
{
      delete PCreg;
      delete instmem;
      delete add4;
      delete addbr;
      delete a1;
      delete mPC;
      delete dec1;
      delete mr;
      delete rfile;
      delete e1;
      delete sl2;
      delete m1;
      delete alu1;
      delete datamem;
      delete m2;
      delete ctrl;
      
      delete hazard_unit;
      delete or_reset_if_id;
      delete or_reset_id2_exe; 
      delete reg_if_id1;
      delete reg_id1_id2;
      delete reg_id2_exe;
      delete reg_exe_mem;
      delete reg_mem_wb;
}
