#include <stdio.h>
#include "blu_impl.h"

blu_impl* pblu = 0;
int bref = 0;


bcthulu* bluCreate(){
return (bcthulu*)blu_impl::InterfaceGet();
}

blu_impl* blu_impl::InterfaceGet(){
if(!pblu) pblu=new blu_impl();
bref++;
return pblu;
}

blu_impl::blu_impl(){
//dummy
int k = 0;
}

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

void blu_impl::System_Start(){
for(;;){
if(bluFrameFunc()) break;
}
}

//this function initiates video ram for immediate use with sprites
//
void blu_impl::GFX_Initiate(){
videoSetMode(MODE_0_2D);
vramSetMainBanks(VRAM_A_MAIN_SPRITE,VRAM_B_MAIN_SPRITE,VRAM_C_LCD,VRAM_D_LCD);
}


void blu_impl::GFX_LDSprite(bluSprite* bsp){
u16* gfx;
oamInit(&oamMain, bsp->sm, false);
gfx = oamAllocateGfx(&oamMain, bsp->sz, bsp->sfmt);
dmaCopy(bsp->tiles, gfx, bsp->tlen);
dmaCopy(bsp->pal, SPRITE_PALETTE, bsp->plen);
bsp->gfx = gfx;
}


//update this function first it's the ugliest and most unwieldly
//maybe add bsprite param?
void blu_impl::GFX_BltSpr(bluSprite* bsp){
oamSet(&oamMain,bsp->id,bsp->x,bsp->y,bsp->priority,0,SpriteSize_64x64,SpriteColorFormat_256Color,bsp->gfx,0,false,false,false,false,false);
oamUpdate(&oamMain);
}

//ANIMATION
//creates one frame and points the next frame to itself
//The tail will always point to the head when you add frames
void blu_impl::GFX_InitAnimationFrames(bluAnimation* ban, u16 frames){
ban->palList = (const u16**)malloc(sizeof(u16*)*frames);
ban->tileList = (const u32**)malloc(sizeof(u32*)*frames);
ban->frames = frames;
}


int blu_impl::GFX_AddAnimationFrame(bluAnimation* ban, u16 index, const u32* tile, const u16* pal){
if(index >= ban->frames)
return -1; //frame index out of range

else{
ban->palList[index] = pal;
ban->tileList[index] = tile;
return 0;
}

}

void blu_impl::GFX_PlayAnimation(bluSprite* bsp, bluAnimation* ban){
if(bsp->frame >= ban->frames)
bsp->frame = 0;

bsp->tiles = ban->tileList[bsp->frame];
bsp->pal = ban->palList[bsp->frame];
bsp->frame++;
}


