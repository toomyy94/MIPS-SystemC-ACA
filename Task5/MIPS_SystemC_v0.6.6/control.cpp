
#include "control.h"

/**
 * \c control module callback function.
 */
void control::entry()
{
  switch(opcode.read()) {
    case 0: // R-format
            RegDst.write(1);  
            ALUSrc.write(0);
	    MemtoReg.write(0);
	    RegWrite.write(1);
	    MemRead.write(0);
	    MemWrite.write(0);
	    switch(funct.read()) {
               case 8:  Jump.write(4); // jr
                        Jreg.write(0);
                        break;
               case 9:  ALUOp.write(3);
                        Jump.write(6); // jalr
                        Jreg.write(1);
                        break;
	       case 32: ALUOp.write(2);
                        Jump.write(0);
                        Jreg.write(0);
	                break;
	       case 34: ALUOp.write(6);
                        Jump.write(0);
                        Jreg.write(0);
	                break;
	       case 36: ALUOp.write(0);
                        Jump.write(0);
                        Jreg.write(0);
	                break;
	       case 37: ALUOp.write(1);
                        Jump.write(0);
                        Jreg.write(0);
	                break;
	       case 42: ALUOp.write(7);
                        Jump.write(0);
                        Jreg.write(0);
	                break;
               default: ALUOp.write(0); 
                        Jump.write(0);
                        Jreg.write(0);
                        break; 
		}
	    break;
    case  2: // Jump
            ALUSrc.write(0);
	    RegWrite.write(0);
	    MemRead.write(0);
	    MemWrite.write(0);
	    Jump.write(3);
        Jreg.write(0);
	    ALUOp.write(0);
	    break;
     case  3: // jal
	     MemtoReg.write(0);
             RegDst.write(1);  
             ALUSrc.write(0);
	     RegWrite.write(1);
	     MemRead.write(0);
	     MemWrite.write(0);
	     Jump.write(5);
             Jreg.write(1);
	     ALUOp.write(3);
	     break;
    case  4: // beq
            ALUSrc.write(0);
	    RegWrite.write(0);
	    MemRead.write(0);
	    MemWrite.write(0);
	    Jump.write(1);
            Jreg.write(0);
	    ALUOp.write(0);
	    break;
    case  5: // bne
            ALUSrc.write(0);
	    RegWrite.write(0);
	    MemRead.write(0);
	    MemWrite.write(0);
	    Jump.write(2);
            Jreg.write(0);
	    ALUOp.write(0);
	    break;

	case  6: // bltz
            ALUSrc.write(0);
	    RegWrite.write(0);
	    MemRead.write(0);
	    MemWrite.write(0);
	    Jump.write(6);
            Jreg.write(0);
	    ALUOp.write(0);
	    break;

	case  7: // bgt
            ALUSrc.write(0);
	    RegWrite.write(0);
	    MemRead.write(0);
	    MemWrite.write(0);
	    Jump.write(5);
            Jreg.write(0);
	    ALUOp.write(0);
	    break;
    case 35: // lw
            RegDst.write(0); 
            ALUSrc.write(1);
	    MemtoReg.write(1);
	    RegWrite.write(1);
	    MemRead.write(1);
	    MemWrite.write(0);
	    Jump.write(0);
            Jreg.write(0);
	    ALUOp.write(2);
	    break;
    case 43: // sw
            ALUSrc.write(1);
	    RegWrite.write(0);
	    MemRead.write(0);
	    MemWrite.write(1);
	    Jump.write(0);
            Jreg.write(0);
	    ALUOp.write(2);
	    break;
   default: // Unknown opcode
            fprintf(stderr,"ERROR: Illegal opcode\n");
	    //assert(0);
	    break;
    }
}
