

/*---------------------------------------------------------------------------------

	Simple console print demo
	-- dovoto
Thank you dovoto. This has become my personal sandbox for toying with babycthulu
It's become something else.
---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include "babycthulu/include/babycthulu.h"
#include "tsprite.h"
#include "sgun.h"

//---------------------------------------------------------------------------------
//unsigned short* g_gfx;
bcthulu* tblu; //long pointer to cthulu
bluSprite bsp;
bluAnimation tban;
bluAnimation tban2;
bluSprite bsp2;

bool FrameProc(bluVent bluMe);
bool DrawGLScene();
static unsigned int rtri;
static unsigned int rquad;

int main(void) {
//---------------------------------------------------------------------------------
	
        tblu = bluCreate();
	//create an interface to sprite data
	bsp.init = 0;
	bsp.framerate = 17;
	bsp.counter = 0;
	bsp.frame = 0;
	bsp.tiles = sgunTiles;
	bsp.tlen = 64*32;
	bsp.pal = sgunPal;
	bsp.plen = sgunPalLen;
	bsp.sm = SpriteMapping_1D_128;
	bsp.sz = SpriteSize_64x32;
	bsp.sfmt = SpriteColorFormat_256Color;
	bsp.x = 32;
	bsp.y = 32;
	bsp.id = 0;
	bsp.priority = 0;
	/*
	bsp2.tiles = tspriteTiles;
	bsp2.tlen = tspriteTilesLen;
	bsp2.pal = tspritePal;
	bsp2.plen = tspritePalLen;
	bsp2.sm = SpriteMapping_Bmp_1D_128;
	bsp2.sz = SpriteSize_64x64;
	bsp2.sfmt = SpriteColorFormat_256Color;
	bsp2.x = 128;
	bsp2.y = 80;
	bsp2.id = 1;
	bsp2.priority = 1;
	*/
	tblu->Input_Init();
	tblu->GFX_Initiate();	
	tblu->GFX_LDSprite(&bsp);
	tblu->GFX_InitAnimationFrames(&tban, 2);
//	tblu->GFX_InitAnimationFrames(&tban2, 3);
	tblu->GFX_AddAnimationFrame(&tban, 0, sgunTiles, sgunPal, sgunTilesLen, sgunPalLen);
	tblu->GFX_AddAnimationFrame(&tban, 1, sgunTiles+512, sgunPal, sgunTilesLen, sgunPalLen);
//	tblu->GFX_AddAnimationFrame(&tban, 2, sspinTiles+512, sspinPal, sspinTilesLen, sspinPalLen);
//	tblu->GFX_AddAnimationFrame(&tban2, 0, sspinTiles+512+256, sspinPal, sspinTilesLen, sspinPalLen);
//	tblu->GFX_AddAnimationFrame(&tban2, 1, sspinTiles+1024, sspinPal, sspinTilesLen, sspinPalLen);
//	tblu->GFX_AddAnimationFrame(&tban2, 2, sspinTiles+1024+256, sspinPal, sspinTilesLen, sspinPalLen);
//
	tblu->System_SetFunc(FrameProc, BLUFRAMFUNC);

	consoleDemoInit();  //setup the sub screen for printing
//	tblu->GFX_BltSpr(&bsp);
	tblu->System_Start();
//        while(true){
//	FrameProc();
//{	


	tblu->GFX_Init3DDevice();
	while(1){
	DrawGLScene();
	glFlush(0);	

	}

/*	tblu->Release();
	while(1) {

		touchRead(&touch);
		iprintf("Touch y = %04i, %04i\n", touch.rawy, touch.py);

		swiWaitForVBlank();
	}

*/	return 0;
}


bool FrameProc(bluVent bluMe){
tblu->Input_KeysPressed();
if(bluMe.msg == PWR_ON){
printf("Input Powered On \n");
}
if(bluMe.msg == NO_MSG){
iprintf("\x1b[20;16H Idling! \n");
}
if(bluMe.msg == NULL_MSG){
printf("EOQ! \n");
}
if(bluMe.msg == KEYPRESS){
printf("KEY PRESS!! \n");
if(bluMe.keys == KEY_A){
for(int i = 0; i < 150; i++){
tblu->GFX_PlayAnimation(&bsp, &tban);
tblu->GFX_BltSpr(&bsp);
swiWaitForVBlank();
}
}
if(bluMe.keys == KEY_B){
for(int i = 0; i < 150; i++){
//tblu->GFX_PlayAnimation(&bsp, &tban2);
tblu->GFX_BltSpr(&bsp);
swiWaitForVBlank();
}
}
if(bluMe.keys == KEY_DOWN){
bluVent b = {QUIT};
tblu->Input_PushEvent(b);
}
}
if(bluMe.msg == QUIT){
return true;
}
//this is just a test hope it works, if program, idles wait for input
return false;
}

bool DrawGLScene()                                                                                       // Here's Where We Do All The Drawing
{

       /*ds does this automagicaly*///glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      // Clear Screen And Depth Buffer
        glLoadIdentity();                                                                       // Reset The Current Modelview Matrix
        glTranslatef(-1.5f,0.0f,-6.0f);                                         // Move Left 1.5 Units And Into The Screen 6.0
        glRotatef(rtri,0.0f,1.0f,0.0f);                                         // Rotate The Triangle On The Y axis ( NEW )
        glColor3f(1, 1, 1);                                                                     // set the vertex color
        glBegin(GL_TRIANGLES);                                                          // Start Drawing A Triangle
                glColor3f(1.0f,0.0f,0.0f);                                              // Set Top Point Of Triangle To Red
                glVertex3f( 0.0f, 1.0f, 0.0f);                                  // First Point Of The Triangle
                glColor3f(0.0f,1.0f,0.0f);                                              // Set Left Point Of Triangle To Green
                glVertex3f(-1.0f,-1.0f, 0.0f);                                  // Second Point Of The Triangle
                glColor3f(0.0f,0.0f,1.0f);                                              // Set Right Point Of Triangle To Blue
                glVertex3f( 1.0f,-1.0f, 0.0f);                                  // Third Point Of The Triangle
        glEnd();                                                                                        // Done Drawing The Triangle
        glLoadIdentity();                                                                       // Reset The Current Modelview Matrix
        glTranslatef(1.5f,0.0f,-6.0f);                                          // Move Right 1.5 Units And Into The Screen 6.0
        glRotatef(rquad,1.0f,0.0f,0.0f);                                        // Rotate The Quad On The X axis ( NEW )
        glColor3f(0.5f,0.5f,1.0f);                                                      // Set The Color To Blue One Time Only
        glBegin(GL_QUADS);                                                                      // Draw A Quad
                glVertex3f(-1.0f, 1.0f, 0.0f);                                  // Top Left
                glVertex3f( 1.0f, 1.0f, 0.0f);                                  // Top Right
                glVertex3f( 1.0f,-1.0f, 0.0f);                                  // Bottom Right
                glVertex3f(-1.0f,-1.0f, 0.0f);                                  // Bottom Left
        glEnd();                                                                                        // Done Drawing The Quad
        rtri+=0.9f;                                                                                     // Increase The Rotation Variable For The Triangle ( NEW )
        rquad-=0.75f;                                                                           // Decrease The Rotation Variable For The Quad ( NEW )
        return false; // Keep Going
	
}

