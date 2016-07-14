#include "forward.h"

void forward::calc()
{
  // FORWARD PARA RS NA FASE ID
  // EXE/MEM -> ID
  if( rs_id.read() == WriteReg_mem.read() && RegWrite_mem.read()==1 )
  {
    //printf(" MUXid2 - rs Forward: forward de exe/mem -> ID: fw_sel_exe = 01:\n");
    rsId1.write(0);
    rsId2.write(1);
  }
  // MEM1/MEM2 -> ID
  //else if( rs_id.read() == WriteReg_mem2.read() && RegWrite_mem2.read()==1 )
  //{
  //  rsId1.write(1);
  //  rsId2.write(0);
  //}
  // MEM2/WB -> ID
  else if( rs_id.read() == WriteReg_wb.read() && RegWrite_wb.read()==1 )
  {
    //printf("MUXid2 - rs Forward: forward de mem/wb -> ID: fw_sel_exe = 10:\n");
    rsId1.write(1);
    rsId2.write(0);
  }
  // NO FORWARD
  else
  {
    //printf("NO Forward");
    rsId1.write(0);
    rsId2.write(0);
  }
  
  // FORWARD PARA RT NA FASE ID
  // EXE/MEM -> ID
  if( rt_id.read() == WriteReg_mem.read() && RegWrite_mem.read()==1 )
  {
    //printf("MUXid2 - rt Forward: forward de exe/mem -> ID: fw_sel_exe = 01:\n");
    rtId1.write(0);
    rtId2.write(1);
  }
  // MEM1/MEM2 -> ID
  //else if( rt_id.read() == WriteReg_mem2.read() && RegWrite_mem2.read()==1 )
  //{
  //  rtId1.write(1);
  //  rtId2.write(0);
  //}
  // MEM2/WB -> ID
  else if( rt_id.read() == WriteReg_wb.read() && RegWrite_wb.read()==1 )
  {
    //printf("MUXexe - rt Forward: forward de mem/wb -> ID: fw_sel_exe = 10:\n");
    rtId1.write(1);
    rtId2.write(0);
  }
  // NO FORWARD
  else
  {
   // printf("NO Forward");
    rtId1.write(0);
    rtId2.write(0);
  }
     
  // EXE/MEM -> EXE
  if( rs_exe.read() == WriteReg_mem.read() && RegWrite_mem.read()==1 )
  {
   // printf("MUXexe - rs Forward: forward de exe/mem -> EXE: fw_sel_exe = 01:\n");
    rsExe1.write(0);
    rsExe2.write(1);
  }
  // MEM1/MEM2 -> EXE
  //else if( rs_exe.read() == WriteReg_mem2.read() && RegWrite_mem2.read()==1 )
  //{
  //  rsExe1.write(1);
  //  rsExe2.write(0);
  //}
  // MEM2/WB -> EXE
  else if( rs_exe.read() == WriteReg_wb.read() && RegWrite_wb.read()==1 )
  {
   // printf("MUXexe - rs Forward: forward de mem/wb -> EXE: fw_sel_exe = 10:\n");
    rsExe1.write(1); //contrario ???
    rsExe2.write(0);
  }
  // NO FORWARD
  else
  {
    //printf("NO Forward");
    rsExe1.write(0);
    rsExe2.write(0);
  }
  
  // FORWARD PARA RT NA FASE EXE
  // EXE/MEM -> EXE
  if( rt_exe.read() == WriteReg_mem.read() && RegWrite_mem.read()==1 )
  {
   // printf("MUXexe - rt Forward: forward de exe/mem -> EXE: fw_sel_exe = 01:\n");
    rtExe1.write(0);
    rtExe2.write(1);
  }
  // MEM1/MEM2 -> EXE
  //else if( rt_exe.read() == WriteReg_mem2.read() && RegWrite_mem2.read()==1 )
  //{
  //  rtExe1.write(1);
  //  rtExe2.write(0);
  //}
  // MEM2/WB -> EXE
  else if( rt_exe.read() == WriteReg_wb.read() && RegWrite_wb.read()==1 )
  {
    //printf("MUXexe - rt Forward: forward de MEM2/WB -> EXE: fw_sel_exe = 10:\n");
    rtExe1.write(1); // contrario ???
    rtExe2.write(0);
  }
  // NO FORWARD
  else
  {
   // printf("NO Forward");
    rtExe1.write(0);
    rtExe2.write(0);
  }
	 

	 
  // FORWARD PARA RT NA FASE MEM
  // MEM1/MEM2 -> MEM 
  //pode n haver?
  //if( MemWrite.read() && rt_mem.read() == WriteReg_mem2.read() && RegWrite_mem2.read()==1  )
  //{
   //rtMem1.write(0);
    //rtMem2.write(1);
  //}
 // MEM2/WB -> MEM
 	//if( MemWrite.read() && rt_mem.read() == WriteReg_wb.read() && RegWrite_wb.read()==1  )
  //{
   // selMem.write(1);
    //rtMem2.write(0);
  //}

  //BRUNO VER ISTO!!!
  if(rt_mem.read()!=0 && rt_mem.read() == WriteReg_wb.read()){
    //printf("MUX5 - rt Forward: datamem regb_mem: forward de WriteReg_wb: %d. fw_sel_mem1: 1\n",(unsigned)WriteReg_wb.read());
    selMem.write(1); //memread_wb propagar?? memwrite_mem?? 
  } 
  else{
    //printf("Forward: datamem regb_mem: sem forwards\n");
    selMem.write(0);
  } 

        //selMem.write(0);
        //rtExe2.write(0);
        //rtExe1.write(0);
        //rtId2.write(0);
        //rtId1.write(0);
  
  // NO FORWARD
  //else
  //{
  //  rtMem1.write(0);
  //  rtMem2.write(0);
  //}


  //if (!MemWrite.read()){
  //  if(rt_mem.read()==1 && rt_mem.read() == WriteReg_wb.read()  /*MemToReg_wb.read()==1*/ && MemWrite.read()==1)
  //    selMem.write(0); 
  //}
}
