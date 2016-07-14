// MIPSmods.cpp


#include "../mips.h"
#include "modview.h"

#include "MIPSmods.h"
//Added by qt3to4:
#include <QPixmap>
#include <QCloseEvent>

#define MODSTEPX 160   // horizontal spacing of module views

/**
 * MIPSmods contructor.
 * Creates and inicializes the widgets that display the
 * port values of intruction memory, control, decode, register file,
 * alu and data memory.
 */
MIPSmods::MIPSmods( mips &m, QWidget* parent,  const char* name, Qt::WFlags fl )
    : Q3ScrollView( parent, name, fl ), mips1(m)
{   
    //resize viewport
    resizeContents(820+5,194+5);

    resize( QSize(1280,194+10) ); 
    setMaximumSize( QSize(1280,280+10) ); 

    setIcon(QPixmap("mips.xpm"));

    setVScrollBarMode(Auto);
    setHScrollBarMode(Auto);

    //imemview
    ModView *imemView=new ModView("imem",viewport());
    imemView->addPort(mips1.instmem->addr, "PC");
    imemView->addPort(mips1.instmem->inst, "inst");
    addChild(imemView,5,5);
    
    connect(this,SIGNAL(updateModules()), imemView, SLOT(redrawModule()));

    //ctrlview
    ModView *ctrlView=new ModView("Control",viewport());
    ctrlView->addPort(mips1.ctrl->opcode,  "opcode");
    ctrlView->addPort(mips1.ctrl->funct,   "funct");
    ctrlView->addPort(mips1.ctrl->RegDst,  "RegDst");
    ctrlView->addPort(mips1.ctrl->Jump,  "Jump");
    ctrlView->addPort(mips1.ctrl->MemRead, "MemRead");
    ctrlView->addPort(mips1.ctrl->MemWrite,"MemWrite");
    ctrlView->addPort(mips1.ctrl->MemtoReg,"MemtoReg");
    ctrlView->addPort(mips1.ctrl->ALUSrc,  "ALUSrc");
    ctrlView->addPort(mips1.ctrl->ALUOp,   "ALUOp");
    ctrlView->addPort(mips1.ctrl->RegWrite,"RegWrite");
    addChild(ctrlView,5+MODSTEPX*1,5);
    
    connect(this,SIGNAL(updateModules()), ctrlView, SLOT(redrawModule()));

    //rfileview
    ModView *rfileView=new ModView("regfile",viewport());
    rfileView->addPort(mips1.rfile->reg1, "r1");
    rfileView->addPort(mips1.rfile->reg2, "r2");
    rfileView->addPort(mips1.rfile->regwrite, "rw");
    rfileView->addPort(mips1.rfile->data1, "r1_out");
    rfileView->addPort(mips1.rfile->data2, "r2_out");
    rfileView->addPort(mips1.rfile->datawr, "rw_in");
    addChild(rfileView,5+MODSTEPX*2,5);
    
    connect(this,SIGNAL(updateModules()), rfileView, SLOT(redrawModule()));

    //aluView
    ModView *aluView=new ModView("ALU",viewport());
    aluView->addPort(mips1.alu1->din1, "din1");
    aluView->addPort(mips1.alu1->din2, "din2");
    aluView->addPort(mips1.alu1->op, "op");
    aluView->addPort(mips1.alu1->dout, "dout");
    aluView->addPort(mips1.alu1->zero, "zero");
    addChild(aluView,5+MODSTEPX*3,5);

    connect(this,SIGNAL(updateModules()), aluView, SLOT(redrawModule()));

    //dmemview
    ModView *dmemView=new ModView("dmem",viewport());
    dmemView->addPort(mips1.datamem->addr, "addr");
    dmemView->addPort(mips1.datamem->din, "din");
    dmemView->addPort(mips1.datamem->dout, "dout");
    addChild(dmemView,5+MODSTEPX*4,5);
    
    connect(this,SIGNAL(updateModules()), dmemView, SLOT(redrawModule()));

    //mux4view
    ModView *mux4View=new ModView("mux4",viewport());
    mux4View->addPort(mips1.mPC->sel, "sel");
    mux4View->addPort(mips1.mPC->din0, "din0");
    mux4View->addPort(mips1.mPC->din1, "din1");
    mux4View->addPort(mips1.mPC->din2, "din2");
    mux4View->addPort(mips1.mPC->din3, "din3");
    mux4View->addPort(mips1.mPC->dout, "dout");
    addChild(mux4View,5+MODSTEPX*5,5);
    
    connect(this,SIGNAL(updateModules()), mux4View, SLOT(redrawModule()));

    //mux4view
    ModView *fwdv=new ModView("forwd",viewport());
    fwdv->addPort(mips1.fwd->rsId1, "rsId1");
    fwdv->addPort(mips1.fwd->rsId2, "rsId2");
    fwdv->addPort(mips1.fwd->rtId1, "rtId1");
    fwdv->addPort(mips1.fwd->rtId2, "rtId2");
    fwdv->addPort(mips1.fwd->rtExe1, "rtExe1");
    fwdv->addPort(mips1.fwd->rtExe2, "rtExe2");
    fwdv->addPort(mips1.fwd->rsExe1, "rsExe1");
    fwdv->addPort(mips1.fwd->rsExe2, "rsExe2");
    fwdv->addPort(mips1.fwd->selMem, "selMem");

    addChild(fwdv,5+MODSTEPX*6,5);
    
    connect(this,SIGNAL(updateModules()), fwdv, SLOT(redrawModule()));

    setFocusPolicy(Qt::StrongFocus);
}

/*  
 *  Destroys the object and frees any allocated resources
 */
MIPSmods::~MIPSmods()
{
    // no need to delete child widgets, Qt does it all for us
}

void MIPSmods::closeEvent(QCloseEvent *e)
{
   e->ignore();  //dont close
}
