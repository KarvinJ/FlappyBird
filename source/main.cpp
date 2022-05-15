/*---------------------------------------------------------------------------------

	$Id: main.cpp,v 1.13 2008-12-02 20:21:20 dovoto Exp $

	Simple console print demo
	-- dovoto


---------------------------------------------------------------------------------*/
#include <nds.h>

#include <gl2d.h>

#include <stdio.h>

#define WIDTH 256

#define HEIGHT 192

#define GRAVITY 1

#define CPIPE pipes[i]

struct pipe{
	int x = 0;
	int height = 0;
};

struct pipe pipes[4];

bool ifbirdbad = false;

float birdy = HEIGHT/2;
float birda = 0;

int frame;
int score;



float gap = 100	;
int space_gap = 100;

bool valueInRange(int value, int min, int max)
{ return (value >= min) && (value <= max); }

//---------------------------------------------------------------------------------
void Vblank() {
//---------------------------------------------------------------------------------

	frame++;

	if(frame % 75 == 0){
		score += 10;
		if(gap != 50){
			gap = gap - 0.4;
		}

	}

}


	
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	vramSetBankA( VRAM_A_TEXTURE );     
	vramSetBankE(VRAM_E_TEX_PALETTE);  // Allocate VRAM bank for all the palettes

	start:

	//define pipes

	pipes[0].height = 50;
	pipes[1].height = 25;
	pipes[2].height = 50;
	pipes[3].height = 75;

	pipes[0].x = space_gap + 30;
	pipes[1].x = space_gap * 2 + 30;
	pipes[2].x = space_gap * 3 + 30;
	pipes[3].x = space_gap * 4 + 30;

	//Begin setup of the screen
	videoSetMode( MODE_5_3D );
	glScreen2D();
	//now in gl mode :)


	irqSet(IRQ_VBLANK, Vblank);

	consoleDemoInit();

	iprintf("Flappy Bird V 1.0\n");

	while(1) {
	
		swiWaitForVBlank();
		scanKeys();
		int keys = keysDown();
		if (keys & KEY_START) break;

		if (keys & KEY_A || keys & KEY_UP){
			birda = -7;	
		}

		//Update Code
		birda = birda + GRAVITY / 1.5;
		birdy = birdy + birda;

		if(birdy >= HEIGHT -10){
			birdy = HEIGHT -10;
		}

		//Now doing real drawing

		glBegin2D();

		glBoxFilled(0,0,WIDTH,HEIGHT,RGB8(126,212,230));

		glBoxFilled(0,HEIGHT-5,WIDTH,HEIGHT,RGB8(30, 160, 39));

		for(int i = 0; i < 4; i++){
			glBoxFilled(
				pipes[i].x-1, pipes[i].height,
				pipes[i].x+25, 0,
				RGB15(0,255,0)
			);


			//if (RectA.X1 < RectB.X2 && RectA.X2 > RectB.X1 &&
			//RectA.Y1 > RectB.Y2 && RectA.Y2 < RectB.Y1) 

		

			if (!valueInRange(birdy,CPIPE.height,HEIGHT)){
				if(CPIPE.x+8 > 20 && CPIPE.x+8 < 30){
					ifbirdbad = true;
				}
				if(CPIPE.x > 20 && CPIPE.x < 30){
					ifbirdbad = true;
				}
				if(CPIPE.x+16 > 20 && CPIPE.x+16 < 30){
					ifbirdbad = true;
				}
				if(CPIPE.x+25 > 20 && CPIPE.x+25 < 30){
					ifbirdbad = true;
				}
			}

			if (!valueInRange(birdy,0,CPIPE.height+gap)){
				if(CPIPE.x+8 > 20 && CPIPE.x+8 < 30){
					ifbirdbad = true;
				}
				if(CPIPE.x > 20 && CPIPE.x < 30){
					ifbirdbad = true;
				}
				if(CPIPE.x+16 > 20 && CPIPE.x+16 < 30){
					ifbirdbad = true;
				}
				if(CPIPE.x+25 > 20 && CPIPE.x+25 < 30){
					ifbirdbad = true;
				}
			}

			glBoxFilled(
				pipes[i].x, pipes[i].height + gap,
				pipes[i].x+25, HEIGHT,
				RGB15(0,255,0)
			);

			if (pipes[i].x+25 <= 0){
				pipes[i].x = (space_gap * 4) - 25;
				pipes[i].height = rand() % (100 + 1 - 15) + 15;
			}

			pipes[i].x = pipes[i].x - 2;



		}

		if(ifbirdbad){
			glBoxFilled(
			 20, birdy,
			 30, birdy+10,
			 RGB15(255,0,0)
			);

			//Restart game

			ifbirdbad = false;
			frame = 0;
			score = 0;
			birdy = HEIGHT/2;
			birda = 0;
			gap = 100;
			//main();

			goto start;
		}else{
			glBoxFilled(
			 20, birdy,
			 30, birdy+10,
			 RGB15(89,89,0)
			);
		}



        glEnd2D();
   
        glFlush(0);                    
        swiWaitForVBlank();

		iprintf("\x1b[4;-1H Score :%d",score);

	}

	return 0;
}
