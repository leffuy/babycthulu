

/*---------------------------------------------------------------------------------

	Simple console print demo
	-- dovoto
Thank you dovoto. This has become my personal sandbox for toying with babycthulu
It's become something else.
---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include "../../../../include/babycthulu.h"
#include "tsprite.h"
#include "sgun.h"

//---------------------------------------------------------------------------------
//unsigned short* g_gfx;
bcthulu* tblu; //long pointer to cthulu
bluSprite bsp;
bluAnimation tban;
bluAnimation tban2;
bluSprite bsp2;

void initRes(void);
bool FrameProc(bluVent bluMe);
bool DrawGLScene();
static unsigned int rtri;
static unsigned int rquad;

int main(void) {
//---------------------------------------------------------------------------------
	
        tblu = bluCreate();
	initRes();
	bsp = *((bluSprite*)tblu->System_ResourceFactory());
	tblu->Input_Init();
	tblu->GFX_Initiate();	
	tblu->GFX_LDSprite(&bsp);
	tblu->GFX_InitAnimationFrames(&tban, 2);
	tblu->GFX_AddAnimationFrame(&tban, 0, sgunTiles, sgunPal, sgunTilesLen, sgunPalLen);
	tblu->GFX_AddAnimationFrame(&tban, 1, sgunTiles+512, sgunPal, sgunTilesLen, sgunPalLen);
	tblu->System_SetFunc(FrameProc, BLUFRAMFUNC);
	consoleDemoInit();  //setup the sub screen for printing
	tblu->System_Start();

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

void initRes(void){
	//this global wrapper allows the factory to create your resources
	//remember tblu likes to be global
	bluWrapper* g_Wrap = tblu->System_GetWrapperHandle();
	g_Wrap->sprW.init = 0;
	g_Wrap->sprW.framerate = 18;
	g_Wrap->sprW.counter = 0;
	g_Wrap->sprW.frame = 0;
	g_Wrap->sprW.tiles = sgunTiles;
	g_Wrap->sprW.tlen = 64*32;
	g_Wrap->sprW.pal = sgunPal;
	g_Wrap->sprW.plen = sgunPalLen;
	g_Wrap->sprW.sm = SpriteMapping_1D_128;
	g_Wrap->sprW.sz = SpriteSize_64x32;
	g_Wrap->sprW.sfmt = SpriteColorFormat_256Color;
	g_Wrap->sprW.x = 32;
	g_Wrap->sprW.y = 32;
	g_Wrap->sprW.id = 0;
	g_Wrap->sprW.priority = 0;
}

bool FrameProc(bluVent bluMe){
tblu->Input_KeysPressed();
if(bluMe.msg == PWR_ON){
}
if(bluMe.msg == NO_MSG){
}
if(bluMe.msg == NULL_MSG){
printf("EOQ! \n");
}
if(bluMe.msg == KEYPRESS){
if(bluMe.keys == KEY_A){
tblu->System_Msg("A Pressed!");
for(int i = 0; i < 150; i++){
tblu->GFX_PlayAnimation(&bsp, &tban);
tblu->GFX_BltSpr(&bsp);
swiWaitForVBlank();
}
}
if(bluMe.keys == KEY_B){
tblu->System_Msg("B Pressed!");
//tblu->GFX_PlayAnimation(&bsp, &tban2);
tblu->GFX_BltSpr(&bsp);
swiWaitForVBlank();
}
if(bluMe.keys == KEY_UP){
tblu->System_LnUp();
}

if(bluMe.keys == KEY_DOWN){
tblu->System_LnDn();
//bluVent b = {QUIT};
//tblu->Input_PushEvent(b);
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

