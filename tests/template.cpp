

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
#include "sspin1.h"
#include "sspin2.h"
#include "sspin3.h"
#include "sspin4.h"

//---------------------------------------------------------------------------------
//unsigned short* g_gfx;
bcthulu* tblu; //long pointer to cthulu
bluSprite bsp;
bluAnimation tban;
bluSprite bsp2;

bool FrameProc();
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
	bsp.tiles = sspin1Tiles;
	bsp.tlen = 32*32;
	bsp.pal = sspin1Pal;
	bsp.plen = sspin1PalLen;
	bsp.sm = SpriteMapping_1D_128;
	bsp.sz = SpriteSize_32x32;
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
	tblu->GFX_InitAnimationFrames(&tban, 4);
	tblu->GFX_AddAnimationFrame(&tban, 0, sspin1Tiles, sspin1Pal, sspin1TilesLen, sspin1PalLen);
	tblu->GFX_AddAnimationFrame(&tban, 1, sspin2Tiles, sspin2Pal, sspin2TilesLen, sspin2PalLen);
	tblu->GFX_AddAnimationFrame(&tban, 2, sspin3Tiles, sspin3Pal, sspin3TilesLen, sspin3PalLen);
	tblu->GFX_AddAnimationFrame(&tban, 3, sspin4Tiles, sspin4Pal, sspin4TilesLen, sspin4PalLen);
	tblu->System_SetFunc(FrameProc, BLUFRAMFUNC);

	consoleDemoInit();  //setup the sub screen for printing
//	tblu->GFX_BltSpr(&bsp);
	tblu->System_Start();
//        while(true){
//	FrameProc();
//	frameFunction();
//	}
///	tblu->System_Start();
	


//	tblu->GFX_Init3DDevice();
//	while(1){
//	DrawGLScene();
//	glFlush(0);	

//	}

/*	tblu->Release();
	while(1) {

		touchRead(&touch);
		iprintf("Touch y = %04i, %04i\n", touch.rawy, touch.py);

		swiWaitForVBlank();
	}

*/	return 0;
}


bool FrameProc(){
bluVent bluMe = tblu->Input_PumpQueue();

if(bluMe.msg == KEYPRESS){
for(int i = 0; i < 150; i++){
tblu->GFX_PlayAnimation(&bsp, &tban);
tblu->GFX_LDSprite(&bsp);
tblu->GFX_BltSpr(&bsp);
swiWaitForVBlank();
}
}
swiWaitForVBlank();
tblu->GFX_BltSpr(&bsp);

iprintf("\x1b[10;0HTouch x =\n");
//this is just a test hope it works, if program, idles wait for input
bluMe = tblu->Input_PumpQueue();
tblu->Input_KeysPressed();
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

