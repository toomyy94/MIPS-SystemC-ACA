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

      mPC = new mux4< sc_uint<32> > ("mPC");
      
      mPC->sel(JumpTaken);
      mPC->din0(PC4);
      mPC->din1(BranchTarget); //
      mPC->din2(JumpTarget); // ((PC4 & 0xF0000000) | ((jlabel<<2) & 0x0FFFFFFF))
      mPC->din3(rega_id2); //
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
      dec1->jlabel(jlabel); //add 
      dec1->ra(ra); //add

   
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

      //isto tava em baixo -.-''
      rfile->data1( rega_id2 );
      rfile->data2( regb_id2 );   

      // Selects Register to Write baixo esquerda 

      mr = new mux< sc_uint<5> > ("muxRDst");
      mr->sel(RegDst);
      mr->din0(rt_id2);
      mr->din1(rd_id2);
      mr->dout(WriteRDst);

      //baixo direita jal 
      mr2 = new mux< sc_uint<5> > ("muxRA");
      mr2->sel(Jreg);
      mr2->din0(WriteRDst); // entrada do mux anterior 
      mr2->din1(ra_id2);
      mr2->dout(WriteReg);

      //mux de cima
      muxrs = new mux< sc_uint<32> > ("muxRS");
      muxrs->sel(Jreg);
      muxrs->din0(regdata1);
      muxrs->din1(PC4_id2);
      muxrs->dout(regdata3);


      // Control
      ctrl = new control ("control");

      ctrl->opcode(opcode_id2); 
      ctrl->funct(funct_id2);   
      ctrl->RegDst(RegDst); 
      ctrl->Jump(Jump);
      ctrl->Jreg(Jreg);
      ctrl->MemRead(MemRead);
      ctrl->MemWrite(MemWrite);
      ctrl->MemtoReg(MemtoReg);
      ctrl->ALUOp(ALUOp);
      ctrl->ALUSrc(ALUSrc);
      ctrl->RegWrite(RegWrite);

      
      // Enables Branch
      //isto estava em mem
      //a1 = new andgate ("a1");
      //a1->din1(Branch); //estava branch_mem 
      //a1->din2(comp_res);
      //a1->dout(BranchTaken);

      //Novas cenas po Branch

      //comparador na branch control
      //c1 = new comp("c1");
        
      //c1->op1(regdata1);
      //c1->op2(regdata2);
      //c1->res(comp_res);

      // Branch Control
      jctrl1 = new jumpcontrol("jumpcontrol");

      jctrl1->din1(regdata1);
      jctrl1->din2(regdata2);
      jctrl1->sel(Jump);
      jctrl1->dout(JumpTaken);

      
      // Adds Branch Immediate to Program Counter + 4
      addbr = new add ("addbr");
   
      addbr->op1(PC4_id2);
      addbr->op2(addr_ext);  
      addbr->res(BranchTarget);


      //de exe...
      // shift left 2 imm_ext 
      sl2 = new shiftl2("sl2");
      sl2->din(imm_ext);
      sl2->dout(addr_ext);

      // 16 to 32 bit signed Immediate extension
      e1 = new ext("ext");
      e1->din(imm_id2);
      e1->dout(imm_ext);

      // Calculates JumpTarget
      jcalc = new jumpcalc("jcalc");
      
      jcalc->din0(jlabel_id2);
      jcalc->din1(PC4_id2);
      jcalc->dout(JumpTarget);

}


/**
 * buils EXE stage components 
 */
void mips::buildEXE(void)
{
      // Selects second operand of ALU
      m1 = new mux< sc_uint<32> > ("muxOp");

      m1->sel(ALUSrc_exe);
      m1->din0(m44dout);
      m1->din1(imm_exe);
      m1->dout(ALUIn2);

      // ALU
      alu1 = new alu("alu");

      alu1->din1(m43dout);
      alu1->din2(ALUIn2);
      alu1->op(ALUOp_exe);
      alu1->dout(ALUOut);
      alu1->zero(Zero);

     //sl2 e addb foi para ID
}

/**
 * buils MEM stage components 
 */
void mips::buildMEM(void)
{
      // Data Memory
      datamem = new dmem ("datamem");
   
      datamem->addr(ALUOut_mem);
      datamem->din(RTfwd);
      datamem->dout(MemOut);
      datamem->wr(MemWrite_mem);
      datamem->rd(MemRead_mem);
      datamem->clk(clk);

      // Forwarding mux for RT
      muxRTfwd = new mux< sc_uint<32> > ("muxRTfwd");

      muxRTfwd->sel(selMem);
      muxRTfwd->din0(regb_mem);  //
      muxRTfwd->din1(WriteVal);
      muxRTfwd->dout(RTfwd);
 
      //BRANCH ESTAVA AQUI
}

/**
 * buils WB stage components 
 */
void mips::buildWB(void)
{
      // Selects Result
      m2 = new mux< sc_uint<32> > ("muxRes");
   
      m2->sel(MemtoReg_wb);
      m2->din1(MemOut_wb);
      m2->din0(ALUOut_wb);
      m2->dout(WriteVal);

}

/**
 * Instantiates the pipeline registers and calls other functions to
 * buils stage specific components 
 */
void mips::buildArchitecture(void){

      const4 = 4;
      const1 = true;
      const0 = 0;

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
      //reg_if_id->enable_forwarding(const1);

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

      reg_id1_id2->ra_id1(ra);
      reg_id1_id2->ra_id2(ra_id2);  
      reg_id1_id2->jlabel_id1(jlabel);
      reg_id1_id2->jlabel_id2(jlabel_id2);   



      reg_id1_id2->PC4_id1(PC4_id1); //já não veem para aqui
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
      //reg_id2_exe->PC4_id2(PC4_id2); já não vai para lá, somador em ID1
      //reg_id2_exe->PC4_exe(PC4_exe);
      
      reg_id2_exe->WriteReg_id2(WriteReg);
      reg_id2_exe->WriteReg_exe(WriteReg_exe);
      reg_id2_exe->MemRead_id2(MemRead);
      reg_id2_exe->MemRead_exe(MemRead_exe);
      reg_id2_exe->MemWrite_id2(MemWrite);
      reg_id2_exe->MemWrite_exe(MemWrite_exe);
      reg_id2_exe->MemtoReg_id2(MemtoReg);
      reg_id2_exe->MemtoReg_exe(MemtoReg_exe);
      //reg_id2_exe->Branch_id2(Branch);
      //reg_id2_exe->Branch_exe(Branch_exe);
      reg_id2_exe->RegWrite_id2(RegWrite);
      reg_id2_exe->RegWrite_exe(RegWrite_exe);
      reg_id2_exe->ALUSrc_id2(ALUSrc);
      reg_id2_exe->ALUSrc_exe(ALUSrc_exe);
      reg_id2_exe->ALUOp_id2(ALUOp);
      reg_id2_exe->ALUOp_exe(ALUOp_exe);

      reg_id2_exe->rega_id2(regdata3); //adicionados
      reg_id2_exe->rega_exe(rega_exe);
      reg_id2_exe->regb_id2(regdata2);
      reg_id2_exe->regb_exe(regb_exe);
      
      reg_id2_exe->PC_id2(PC_id2);
      reg_id2_exe->PC_exe(PC_exe);
      reg_id2_exe->valid_id2(valid_id2);
      reg_id2_exe->valid_exe(valid_exe);
      reg_id2_exe->clk(clk);
      reg_id2_exe->reset(reset_id2exe);
      reg_id2_exe->enable(const1);
     // reg_id2_exe->enable_forwarding(const1);

      reg_id2_exe->rs_id2(rs_id2); 
      reg_id2_exe->rs_exe(rs_exe);
      reg_id2_exe->rt_id2(rt_id2); 
      reg_id2_exe->rt_exe(rt_exe);


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
      //reg_exe_mem->Branch_exe(Branch_exe);
      //reg_exe_mem->Branch_mem(Branch_mem);
      reg_exe_mem->RegWrite_exe(RegWrite_exe);
      reg_exe_mem->RegWrite_mem(RegWrite_mem);
      //reg_exe_mem->Zero_exe(Zero);
      //reg_exe_mem->Zero_mem(Zero_mem);
      //reg_exe_mem->BranchTarget_exe(BranchTarget);
      //reg_exe_mem->BranchTarget_mem(BranchTarget_mem);
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
      //reg_exe_mem->enable_forwarding(const1);

      reg_exe_mem->rt_exe(rt_exe);
      reg_exe_mem->rt_mem(rt_mem);

      buildMEM();
      
      //reg_mem_wb
      reg_mem_wb = new reg_mem_wb_t("reg_mem_wb");
      reg_mem_wb->aluOut_mem(ALUOut_mem);
      reg_mem_wb->aluOut_wb(ALUOut_wb);
      reg_mem_wb->memOut_mem(MemOut);
      reg_mem_wb->memOut_wb(MemOut_wb);
      
      reg_mem_wb->MemRead_mem(MemRead_mem);
      reg_mem_wb->MemRead_wb(MemRead_wb);

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
      hazard_unit->MemRead_exe(MemRead_exe); 
      hazard_unit->MemRead_mem(MemRead_mem); 

      hazard_unit->WriteReg_exe(WriteReg_exe);
      hazard_unit->RegWrite_exe(RegWrite_exe);
      hazard_unit->mem_read(MemRead); //thisislw
      hazard_unit->RegWrite_mem(RegWrite_mem);
      hazard_unit->WriteReg_mem(WriteReg_mem);
	hazard_unit->RegWrite_wb(RegWrite_wb); //add
      hazard_unit->WriteReg_wb(WriteReg_wb); //add
      //hazard_unit->RegWrite(RegWrite); 
      hazard_unit->JumpTaken(JumpTaken); //thisisbranch
      //hazard_unit->WriteReg_id2(WriteReg);  
      hazard_unit->enable_pc(enable_pc); //
      hazard_unit->enable_ifid1(enable_ifid1); //
      hazard_unit->enable_id1id2(enable_id1id2);
      hazard_unit->enable_regfile(enable_regfile); //
      hazard_unit->reset_ifid1(reset_haz_ifid1);  //

      hazard_unit->reset_id2exe(reset_haz_id2exe); // 

      // new
     // hazard_unit->thisIsSW(MemWrite); 
      //hazard_unit->lw_exe(MemWrite_exe); 
     // hazard_unit->lw_mem(MemWrite_mem);
      hazard_unit->Jump(Jump);  //branchJR



      fwd = new forward ("fwd"); 
      fwd->rs_id(rs_id2);
      fwd->rt_id(rt_id2); 
      fwd->rs_exe(rs_exe);
      fwd->rt_exe(rt_exe); 
      fwd->rt_mem(rt_mem); 
      fwd->WriteReg_mem(WriteReg_mem); 
      //fwd->WriteReg_mem2(WriteReg_mem2);
      fwd->WriteReg_wb(WriteReg_wb);
     // fwd->WriteReg_aux(WriteReg_aux);
      fwd->RegWrite_mem(RegWrite_mem);
      //fwd->RegWrite_mem2(RegWrite_mem2); 
      fwd->RegWrite_wb(RegWrite_wb); 
      //fwd->RegWrite_aux(RegWrite_aux); 
      fwd->MemWrite(MemWrite_mem);
     // fwd->MemToReg_wb(MemToReg_wb);
      fwd->MemRead_wb(MemRead_wb); 

      //saidas
  
      fwd->rtId1(rtId1); 
      fwd->rtId2(rtId2); 
      fwd->rsId1(rsId1); 
      fwd->rsId2(rsId2); 
      fwd->rtExe1(rtExe1);
      fwd->rtExe2(rtExe2); 
      fwd->rsExe1(rsExe1); 
      fwd->rsExe2(rsExe2); 
      //fwd->rtMem(rtMem);// ja nao ha 
      fwd->selMem(selMem); 
      //fwd->rtMem2(rtMem2);




      mx41 = new mux4bool < sc_uint<32> > ("mx41");
      mx41->din0(rega_id2 );
      mx41->din1(ALUOut_mem);
      mx41->din2(WriteVal);
      mx41->din3(const0);
      mx41->dout(regdata1);
      mx41->sel1(rsId1);
      mx41->sel2(rsId2);
        
      mx42 = new mux4bool < sc_uint<32> > ("mx42");
      mx42->din0(regb_id2);
      mx42->din1(ALUOut_mem);
      mx42->din2(WriteVal);
      mx42->din3(const0);
      mx42->dout(regdata2);
      mx42->sel1(rtId1);
      mx42->sel2(rtId2);
        
      mx43 = new mux4bool < sc_uint<32> > ("mx43");
      mx43->din0(rega_exe);
      mx43->din1(ALUOut_mem);
      mx43->din2(WriteVal);
      mx43->din3(const0);
      mx43->dout(m43dout);
      mx43->sel1(rsExe1);
      mx43->sel2(rsExe2);
        
      mx44 = new mux4bool < sc_uint<32> > ("mx44");
      mx44->din0(regb_exe);
      mx44->din1(ALUOut_mem);
      mx44->din2(WriteVal);
      mx44->din3(const0);
      mx44->dout(m44dout);
      mx44->sel1(rtExe1);
      mx44->sel2(rtExe2);


      //cenas
      //entradas
      // forwarding_unit = new forwarding("forwarding_unit");
      // forwarding_unit->rs( rs );
      // forwarding_unit->rt( rt );
      // forwarding_unit->rs_exe( rs_exe );
      // forwarding_unit->rt_exe( rt_exe );
      // forwarding_unit->WriteReg_mem1(WriteReg_mem1);
      // //forwarding_unit->WriteReg_mem2(WriteReg_mem2);
      // forwarding_unit->WriteReg_wb(WriteReg_wb);
      // forwarding_unit->RegWrite_mem1(RegWrite_mem1);
      // //forwarding_unit->RegWrite_mem2(RegWrite_mem2);
      // forwarding_unit->RegWrite_wb(RegWrite_wb);

      // //saidas
      // forwarding_unit->SelRegData1(SelRegData1);
      // forwarding_unit->SelRegData2(SelRegData2);
      // forwarding_unit->SelRegData1_exe(SelRegData1_exe);
      // forwarding_unit->SelRegData2_exe(SelRegData2_exe);
      // forwarding_unit->MemtoReg_mem1(MemtoReg_mem1);
      // //forwarding_unit->MemtoReg_mem2(MemtoReg_mem2);
      // forwarding_unit->MemtoReg_wb(MemtoReg_wb);
      // forwarding_unit->MemWrite_mem1(MemWrite_mem1);
      // forwarding_unit->SelRTfwd(SelRTfwd);
   }

mips::~mips(void)
{
      delete PCreg;
      delete instmem;
      delete add4;
      delete addbr;
      delete mPC;
      delete dec1;
      delete mr;
      delete rfile;
      delete e1;
      delete sl2;
      delete m1;
      delete jctrl1;
      delete alu1;
      delete datamem;
      delete mx42;
      delete mx44;
      delete mx41;
      delete mx43;
      delete ctrl;
      delete fwd;
      delete hazard_unit;
      delete or_reset_if_id;
      delete or_reset_id2_exe; 
      delete reg_if_id1;
      delete reg_id1_id2;
      delete reg_id2_exe;
      delete reg_exe_mem;
      delete reg_mem_wb;
}
