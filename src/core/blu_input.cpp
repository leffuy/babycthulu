//blu_input.cpp
#include <stdio.h>
#include "blu_impl.h"

//this needs to be called to power on the Input Modules this will initialize
//the queue which is resolved by the user to parse msg's sent by the engine
//when interfacing with the hardware (vis-a-vis I/O).
void blu_impl::Input_Init(){
this->StashQueue();
}

//Call this every FrameFunction, or other loop to pump the queue for messages
//this should always be spinning on a thread. But only call once per program
//logic or you'll be casting haste/slow on random events depending on the timing
//of the program
//This will pump the oldest event (the one that needs to be resolved most), out
//via the method's return value. The retrieved event object needs to be dealt
//as after this method returns the event is resolved off the input queue and 
//thus the handler assumes you (the user) has resolved it
//If you receive a NULL_MSG you need to restart the pump by restashing the queue
//this shouldn't happen often unless you are resolving messages way too fast

bluVent blu_impl::Input_PumpQueue(){
if(!headpV){
bluVent bnull;
bnull.msg = NULL_MSG;
return bnull;
}
bluVent current = *headpV;
if(headpV->next){
bluVent* freeme = headpV;
headpV = headpV->next;
free(freeme);
}
return current;
}

//Basic keyPRESSED method. Returns 1 if pressed 0 if left alone. Places "NO_MSG"
//On the queue to indicate an IDLE call will signal "KEYPRESSED" if a key is 
//Pressed and not held. 
int blu_impl::Input_KeysPressed(){
bluVent keyVent;
scanKeys();
unsigned int keys = 0;
keys = keysDown();

if(!keys){
keyVent.msg = NO_MSG;
AddToTailQueue(keyVent);
return 0;
}

keyVent.msg = KEYPRESS;
keyVent.keys = keys; 
AddToTailQueue(keyVent);
return 1;
}

//Same as keys pressed but returns true as long as a key is being held. Resolve
//the event if it fires to figure out what keys are being held/pressed
int blu_impl::Input_KeysHeld(){
bluVent keyVent;
scanKeys();
unsigned int keys = keysHeld();

if(!keys){
keyVent.msg = NO_MSG;
AddToTailQueue(keyVent);
return 0;
}

keyVent.msg = KEYHELD;
keyVent.keys = keys; 
AddToTailQueue(keyVent);
return 1;
}

//Build an event and push it to PushEvent and this will place it onto the tail
//of the queue. Eventually the head will reach it. Don't push too many, the 
//system is built so only on bluVent can be resolved per framefunc
void blu_impl::Input_PushEvent(bluVent bvent){
this->AddToTailQueue(bvent);
}

//powers up the event event engine by creating a head pointer for the queue
//in malloc space and set
void blu_impl::StashQueue(){
headpV = (bluVent*)malloc(sizeof(bluVent));
bluVent* tmpTail = (bluVent*)malloc(sizeof(bluVent));
tmpTail->msg = NO_MSG;
headpV->msg = PWR_ON;
headpV->next = tmpTail;
tailpV = tmpTail;
}

void blu_impl::AddToTailQueue(bluVent pBV){
bluVent* p_tmpV = (bluVent*)malloc(sizeof(bluVent));
p_tmpV->msg = pBV.msg;
p_tmpV->keys = pBV.keys;
p_tmpV->flags = pBV.flags;
tailpV->next = p_tmpV;
tailpV = tailpV->next;
}


