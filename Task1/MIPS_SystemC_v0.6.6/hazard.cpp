	
#include "hazard.h"

/**
 * Callback for the hazrd detection of \c hazard module.
 */
void hazard::detect_hazard()
{
	//data hazards
	if(    rs.read()!=0 && rs.read()==WriteReg_exe.read() && RegWrite_exe.read()==true
	    || rs.read()!=0 && rs.read()==WriteReg_mem.read() && RegWrite_mem.read()==true
	    || rs.read()!=0 && rs.read()==WriteReg_wb.read()  && RegWrite_wb.read()==true
	    || rt.read()!=0 && rt.read()==WriteReg_exe.read() && RegWrite_exe.read()==true && mem_read.read() == false
	    || rt.read()!=0 && rt.read()==WriteReg_mem.read() && RegWrite_mem.read()==true && mem_read.read() == false
	    || rt.read()!=0 && rt.read()==WriteReg_wb.read() && RegWrite_wb.read()==true && mem_read.read() == false ){
		// novo mem_read.read() == false
		// reset 0 enable 0 stall 
		// reset 0 enable 1 normal 
		// reset 1 enable 1 bubble 

		//printf("Bubble exe, Stall before\n");
		enable_pc.write(false); //
		enable_ifid1.write(false); //
		enable_id1id2.write(false); //
		enable_regfile.write(false); //


		reset_ifid1.write(false); // 
		reset_id2exe.write(true); // 		
	}

	else if (BranchTaken.read()==true)
	{
		//printf("Bubble EXE, ID2, Stall asdasd");

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

