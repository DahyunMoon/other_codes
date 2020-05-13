/*
	Compile with Console.c and ‘-pthread –D_LINUX’ flags 
	Ex) gcc Bouncing_Ball.c Console.c –pthread –D_LINUX
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#include <unistd.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0

#include "Console.h"

// use this structure to send parameter to the child threads
typedef struct {
	int width, height;		// screen size
	int x, y;				// current coordinate of the ball
	int dx, dy;				// velocity of the ball
} ThreadParam;

// initialize the parameter for the thread function
void InitParam(ThreadParam *param, int width, int height);

// thread function to move a ball
void* MoveBall(void *vparam);

int repeat = TRUE;

int main(int argc, char *argv[])
{
	// get <# of balls> from command line parameter
	int no_thread = 0;
	if(argc > 1)
		no_thread = atoi(argv[1]);
	if(no_thread == 0)				// for safety
		no_thread = 5;				// default value of <# of threads>

	srand(time(NULL));

	int width = getWindowWidth();
	int height = getWindowHeight() - 1;
	int t = 0;						// thread index

	EnableCursor(0);				// hide cursor

	clrscr();
	printf("screen size = %d x %d\n", width, height);
	printf("Press ESC to quit!\n");

//	initialize location and velocity of ball
	int x = rand() % width + 1;
	int y = rand() % height + 1;
	int dx = rand() % 7 - 3;		// dx in [-3, +3]
	int dy = rand() % 5 - 2;		// dy in [-2, +2]

// TO DO: modify the above code to represent multiple balls
	pthread_t tid[no_thread];
	ThreadParam param[no_thread];
	
	for(t=0; t<no_thread;t++)
		InitParam(&param[t], width, height);

// TO DO: extend the above animation code to animate multiple balls using threads 
	for(t = 0; t < no_thread; t++)
		pthread_create(&tid[t], NULL, MoveBall, &param[t]);
	
	while(getch() != 27)
		MySleep(1000);
	repeat = FALSE;
	
	for(t = 0; t < no_thread; t++)
		pthread_join(tid[t], NULL);
	
	clrscr();
	gotoxy(1, 1);
	printf("Bye!\n");

	EnableCursor(1);			// enable cursor

	return 0;
}

void InitParam(ThreadParam *param, int width, int height)
{
	// TO DO: implement this function to initialize param
		param->width = width;
		param->height = height;
		param->x = rand() % width + 1;
		param->y = rand() % height + 1;
		param->dx = rand() % 7 - 3;
		param->dy = rand() % 5 - 2;
}

void* MoveBall(void *vparam)
{
	ThreadParam *param = (ThreadParam*) vparam;
	int x = param->x;
	int y = param->y;
	int dx = param->dx;
	int dy = param->dy;
	int width = param->width;
	int height = param->height;
	// TO DO: implement this function 
	
	// animate the bouncing ball 
	while(repeat){
		// save current coordinate
		int oldx = x;
		int oldy = y;
		
		// update horizontal coordinate
		x += dx;

		// horizontal bouncing
		if(x <= 0){
			x = 1 + (1 - x);
			dx = -dx;
		} else if(x > width){
			x = width - (x - width) - 1;
			dx = -dx;
		}

		// update vertical coordinate
		y += dy;

		// vertical bouncing
		if(y <= 0){
			y = 1 + (1 - y);
			dy = -dy;
		} else if(y > height){
			y = height - (y - height) - 1;
			dy = -dy;
		}

		// delete previous ball
		PrintXY(oldx, oldy, " ");

		// draw new ball
		PrintXY(x, y, "*");

		// delay
		MySleep(50);
	}
	return NULL;
}

