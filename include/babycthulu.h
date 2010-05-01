
#ifndef BABY_CTHULU____
#define BABY_CTHULU____

#include <nds.h>


typedef bool (*bluCallback)();

struct bluSprite{
int x;
int y;
int fps;
int id;
int priority;
unsigned short* gfx;
const unsigned int* tiles;
const unsigned short* pal;
unsigned int tlen;
unsigned int plen;
SpriteMapping sm;
SpriteSize sz;
SpriteColorFormat sfmt;
};

enum bluFunc{
BLUFRAMFUNC = 0,
BLURENDFUNC = 1
};

struct bluBG{

};

class bcthulu{
public:
virtual void Release() = 0;
virtual void System_Start() = 0;
virtual void System_SetFunc(bluCallback func, bluFunc fblu) = 0;
virtual void GFX_Initiate() = 0;
virtual void GFX_LDSprite(bluSprite* bsp) = 0;
virtual void GFX_BltSpr(bluSprite* bsp) = 0;

};

extern bcthulu* bluCreate();
#endif

