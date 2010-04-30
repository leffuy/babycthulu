
#ifndef BABY_CTHULU____
#define BABY_CTHULU____

#include <nds.h>


typedef bool (*bluCallback)();

struct bluSprite{
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

enum bluGfx{
BLU2DSPRITE = 7,
BLU3DTEX = 8
};

class bcthulu{
public:
virtual void Release() = 0;
virtual void System_Start() = 0;
virtual void System_SetFunc(bluCallback func, bluFunc fblu) = 0;
virtual void GFX_Initiate(bluGfx bgfx) = 0;
virtual u16* GFX_LDSprite(bluSprite bsp) = 0;
virtual void GFX_BltSpr(u16* p_gfx) = 0;

};

extern bcthulu* bluCreate();
#endif

