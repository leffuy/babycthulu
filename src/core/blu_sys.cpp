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
bluFrameFunc = 0;
bluRenderFunc = 0;
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
if(bluRenderFunc) bluRenderFunc();
}
}

//this function initiates video ram for immediate use with sprites
//
void blu_impl::GFX_Initiate(){
videoSetMode(MODE_0_2D);
vramSetMainBanks(VRAM_A_MAIN_SPRITE,VRAM_B_LCD,VRAM_C_LCD,VRAM_D_LCD);
}


void blu_impl::GFX_LDSprite(bluSprite* bsp){
if(bsp->init == 0){
u16* gfx;
oamInit(&oamMain, bsp->sm, false);
gfx = oamAllocateGfx(&oamMain, bsp->sz, bsp->sfmt);
bsp->gfx = gfx;
bsp->init = 1;
}

dmaCopy(bsp->tiles, bsp->gfx, bsp->tlen);
dmaCopy(bsp->pal, SPRITE_PALETTE, bsp->plen);

}


//update this function first it's the ugliest and most unwieldly
//maybe add bsprite param?
void blu_impl::GFX_BltSpr(bluSprite* bsp){
oamSet(&oamMain,bsp->id,bsp->x,bsp->y,bsp->priority,0,bsp->sz,bsp->sfmt,bsp->gfx,0,false,false,false,false,false);
oamUpdate(&oamMain);
}

//ANIMATION
//Once animation is init to some number of frames start adding them
void blu_impl::GFX_InitAnimationFrames(bluAnimation* ban, u16 frames){
ban->palList = (const u16**)malloc(sizeof(u16*)*frames);
ban->tileList = (const u32**)malloc(sizeof(u32*)*frames);
ban->tlen = (u32*)malloc(sizeof(u32)*frames);
ban->plen = (u32*)malloc(sizeof(u32)*frames);
ban->frames = frames;
}

//You need to call this when you're done with your animation to patch memleaks
void blu_impl::GFX_ReleaseAnimationFrames(bluAnimation* ban){
free(ban->palList);
free(ban->tileList);
free(ban->tlen);
free(ban->plen);
ban->frames = 0;
}

//Rest is pretty straight forward
int blu_impl::GFX_AddAnimationFrame(bluAnimation* ban, u16 index, const u32* tile, const u16* pal, u32 tlength, u32 plength){
if(index >= ban->frames)
return -1; //frame index out of range

else{
ban->palList[index] = pal;
ban->tileList[index] = tile;
ban->tlen[index] = tlength;
ban->plen[index] = plength;
return 0;
}

}

void blu_impl::GFX_PlayAnimation(bluSprite* bsp, bluAnimation* ban){
bsp->counter++;
if(bsp->frame >= ban->frames)
bsp->frame = 0;

bsp->tiles = ban->tileList[bsp->frame];
bsp->pal = ban->palList[bsp->frame];
bsp->tlen = ban->tlen[bsp->frame];
bsp->plen = ban->plen[bsp->frame];

bsp->frame += !(bsp->counter%bsp->framerate);
}



//3D graphics implementation utilizes Libnds' GL impl of GFX ports
//See bottledlight.com this is tricky to use

//PLEASE PLEASE PLEASE read this before using. Or see a demo file.
/*
I am trusting libnds 100% on it's implementation of the hardware
thus below is an abstraction of OpenGL to use as an engine. Unlike traditional
GPU's the NDS utilizes a multi-core graphics engine, the 3D core runs 
independently of the 2D engine cores. As such sprites and BG run indpendently.
The engines are driven by the ARM9 clock. When a screen is in 3D mode it is
limited to 6000+ something vertices on the screen at once or something. It's
small, but keep in mind that's around the size of n64. It also has a texturing
/lighting/rendering engine aside from the geometry engine. This engine maps
to VIDEO MEMORY for texturing. So as you use 3D and texture, memory available
to the engine starts decreasing rapidly. BE AWARE OF THAT. THIS IS THE POINT
OF THAT RANT!  
*/


//This function initializes the device to a "default" setting. 
//You may begin sending GL commands to the DS after this function executes
void blu_impl::GFX_Init3DDevice(){
videoSetMode(MODE_0_3D);

// initialize the geometry engine
glInit();

// enable antialiasing
glEnable(GL_ANTIALIAS);

// setup the rear plane
glClearColor(0,0,0,31); // BG must be opaque for AA to work
glClearPolyID(63); // BG must have a unique polygon ID for AA to work
glClearDepth(0x7FFF);

// Set our viewport to be the same size as the screen
glViewport(0,0,255,191);

// this controls your camera 
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(70, 256.0 / 192.0, 0.1, 100);

//ds specific, several attributes can be set here       
glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
// Set the current matrix to be the model matrix
glMatrixMode(GL_MODELVIEW);
}
