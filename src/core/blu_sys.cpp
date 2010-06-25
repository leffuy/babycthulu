//blu_sys.cpp
#include <stdio.h>
#include "blu_impl.h"

blu_impl* pblu = 0;
int bref = 0;

//external function that will create a reference to the blu interface
//API Descriptor:
//This function is called externally ("DLL loaded") and is the only thing static
//in the entire scope, besides the internals which get pushed into the
//hidden implementation which essentially them. This non virtual function forces
//the ARM9 program to create a reference to the virtual table that constantly
//resolves addresses to anything defined in the interface in the main .h. These
//calls are fast as shown by the time lapse in the input resolve table

bcthulu* bluCreate(){
return (bcthulu*)blu_impl::InterfaceGet();
}

//this internal will get your interface reference and keep track of refcounting
//doesn't really need an API decript
blu_impl* blu_impl::InterfaceGet(){
if(!pblu) pblu=new blu_impl();
bref++;
return pblu;
}
//constructor pretty much initializes all the internals when the first reference//to the interface is created. After which these values hold down.
blu_impl::blu_impl(){
//dummy
bluFrameFunc = 0;
bluRenderFunc = 0;
}

//Destroys references until their are no more then completely destroys reference
//to the vtable which frees program RAM 
//API Descriptor:
//Call this until the interface is completely released and cthulu will literally
//exit from main memory. Call Resource_Release to flush the resource table
void blu_impl::Release(){
bref--;
if(!bref){
delete pblu;
pblu = 0;
}
}

void blu_impl::System_SetFunc(bluCallback func, bluFunc fblu){
switch(fblu){
case BLUFRAMFUNC: bluFrameFunc=func; break;
case BLURENDFUNC: bluRenderFunc=func; break;
}
}


//Start construction
//we'll start editing here I'll chuck out these comments when construction is
//completed

//Lets make the System update the keys every round for starters
//Then add size functionality to the queue for maintenance and optimization 
//issues
//Then I guess create an event handler for each type of even to cut down on
//dispatching through a switchbaord

void blu_impl::System_Start(){
for(;;){
bluVent aVent = this->Input_PumpQueue();
//event based handling causes these to hit and get added to history to be dealt
//with later. Causes latency but humans are slow.
if(bluFrameFunc(aVent)) break;
if(bluRenderFunc) bluRenderFunc(aVent);
}
}

//End construction. Make sure to annotate all notes man!




