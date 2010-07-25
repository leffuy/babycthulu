//File: 
//blu_sys.cpp
//Author:
//Stevie Frederick
//Description:
//This file/module encapsulates the piece of the interface that implements the
//system. This includes things like printing to the developer console/log
//setting up the input queue and other standard initializations, such as calling
//the user's frame and render function when the main loop starts.

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
bWrap = (bluWrapper*)malloc(sizeof(bluWrapper));
}

//Destroys references until their are no more then completely destroys reference
//to the vtable which frees program RAM 
//API Descriptor:
//Call this until the interface is completely released and cthulu will literally
//exit from main memory. Call Resource_Release to flush the resource table
void blu_impl::Release(){
bref--;
if(!bref){
free(bWrap);
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

//Finished the basic construction. This function starts the Frame and Render 
//Functions defined by the user. Before every "game cycle" an event is pumped
//from the message queue that have accumulated during the last cycle and passed
//to the Frame and Render Functions to handle. Only one event can be handled per
//cycle on default. Manual calls to pump the queue can be made if the resources
//are available to resolve whatever msg the engine is making
void blu_impl::System_Start(){
//initialize the system log with the system start 
this->blog = (logList*)malloc(sizeof(logList));
blog->logLine = "system start";
logTP = blog;
logSz = 1;
curLn = 1;
printf("0.%d %s",curLn,blog->logLine);
for(;;){
bluVent aVent = this->Input_PumpQueue();
//event based handling causes these to hit and get added to history to be dealt
//with later. Causes latency but humans are slow.
if(bluFrameFunc(aVent)) break;
if(bluRenderFunc) bluRenderFunc(aVent);
}
}

//DevConsole

//Issue a msg out to the console and log it for later use with a ln #
//
void blu_impl::System_Msg(char const* str){
logList* newLine = (logList*)malloc(sizeof(logList));
logTP->next =  newLine;
logTP->next->logLine = str;
logSz++;
curLn++;
logTP = logTP->next;
printf("0.%d %s\n",curLn,logTP->logLine);
}


//reprint some lines x through y to the console after clearing it
void blu_impl::PrintLines(int x, int y){
consoleClear();
logList* frstLn = blog;

if(x > logSz || y > logSz){
printf("Segmentation Fault!\n");
return;
}

for(int i = 1; i <= x; i++){
frstLn = frstLn->next;
}

curLn = x;

for(int i = x; i < y; i++){
printf("0.%d %s\n",curLn,frstLn->logLine);
curLn++;
}

}
void blu_impl::System_LnUp(void){
if(logSz <= 23)
return;

if(curLn > logSz || curLn <=  23)
return;

PrintLines(curLn-23,curLn-1);
}

void blu_impl::System_LnDn(void){
if(logSz <= 23)
return;

if(curLn > logSz || curLn < 23)
return;

PrintLines(curLn-21,curLn+1);
}


//Resource Manager
//This new module is part of the system_wide resource manager

//Note:
//There is some fucking hackery afloat with this, which means there are memory
//leaks to be plugged. Resource factory needs a reference counter just like the other internal impl objects.
bluWrapper* blu_impl::System_GetWrapperHandle(void){
return bWrap;
}


void* blu_impl::System_ResourceFactory(void){
switch(bWrap->typW){
case T_SPRITE:
bluSprite tmp_s = bWrap->sprW;
bluSprite* tp_spr = (bluSprite*)malloc(sizeof(bluSprite));
*tp_spr = tmp_s;
return (void*)tp_spr;
break;
}
return NULL;
}



