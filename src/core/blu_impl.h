#ifndef P_U___
#define P_U___

#include "../../include/babycthulu.h"



class blu_impl: public bcthulu{
public:
virtual void Release();
virtual void System_Start();
virtual void System_SetFunc(bluCallback func, bluFunc fblu);
virtual void GFX_Initiate();
virtual void GFX_LDSprite(bluSprite* bsp);
virtual void GFX_BltSpr(bluSprite* bsp);


//impl specific funcs
static blu_impl* InterfaceGet();

//members
bool (*bluFrameFunc)();
bool (*bluRenderFunc)();

private:
inline blu_impl();
};

extern blu_impl* pblu;

#endif
