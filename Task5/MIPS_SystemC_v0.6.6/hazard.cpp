	
#include "hazard.h"

/**
 * Callback for the hazrd detection of \c hazard module.
 */
void hazard::detect_hazard()
{
	/*data hazards 
	if(    rs.read()!=0 && rs.read()==WriteReg_exe.read() && RegWrite_exe.read()==true
	    || rs.read()!=0 && rs.read()==WriteReg_mem.read() && RegWrite_mem.read()==true
	    || rs.read()!=0 && rs.read()==WriteReg_wb.read()  && RegWrite_wb.read()==true
	    || rt.read()!=0 && rt.read()==WriteReg_exe.read() && RegWrite_exe.read()==true && mem_read.read() == false
	    || rt.read()!=0 && rt.read()==WriteReg_mem.read() && RegWrite_mem.read()==true && mem_read.read() == false
	    || rt.read()!=0 && rt.read()==WriteReg_wb.read() && RegWrite_wb.read()==true && mem_read.read() == false ){
	*/




if( (rs.read() != 0 && rt.read() != 0) &&
            (
              // add $1
             // jr  $1             
             Jump.read()!=0 && rs.read() == WriteReg_exe.read() && RegWrite_exe.read() || //? nop 
             Jump.read()!=0 && rt.read() == WriteReg_exe.read() && RegWrite_exe.read()  /*&& MemWrite.read()== false*/ ||  
             
             // lw $1
             // jr $1
             Jump.read()!=0 && rs.read() == WriteReg_mem.read() && MemRead_mem.read() == true && MemWrite.read()== false ||  
             Jump.read()!=0 && rt.read() == WriteReg_mem.read() && MemRead_mem.read() == true && MemWrite.read()== false ||
             
             // lw  $1
             // add --, $1
             rs.read() == WriteReg_exe.read() && MemRead_exe.read() == true && MemWrite.read()== false ||
             rt.read() == WriteReg_exe.read() && MemRead_exe.read() == true && MemWrite.read()== false 
            ) ) {

		// novo mem_read.read() == false
		// Reset 0 Enable 0 = stall 
		// Reset 0 Enable 1 = normal 
		// Reset 1 Enable 1 = bubble 

		//printf("Bubble exe, Stall before\n");

		enable_pc.write(false); //
		enable_ifid1.write(false); //
		enable_id1id2.write(false); //
		enable_regfile.write(false); //
		reset_ifid1.write(false); // 
		reset_id2exe.write(true); // 		
	}

	else if (JumpTaken.read()!=0)
	{
		//printf("BranchTaken, discard if\n");

		enable_pc.write(true); //
		enable_ifid1.write(true); //
		enable_id1id2.write(true); //
		enable_regfile.write(true); //

		reset_ifid1.write(true); // 
		reset_id2exe.write(false); // 		
	}
	
	else {
		//printf("No bubble, No stall\n");

		enable_pc.write(true); //
		enable_ifid1.write(true); //
		enable_id1id2.write(true); //
		enable_regfile.write(true); //

		reset_ifid1.write(false); // 
		reset_id2exe.write(false); // 		
	}
	
}

