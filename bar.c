// If the comments are hard to read because of the color,
// type ':set background=dark'

/***

	This program moves bars left and right until the user types 'q' key.
	Usage: $ ./bar <no_bar>

	Currently, it moves only one bar.
	Extend this program to move multiple bars using multiple threads.
	- Launch multiple threads, each of which moves a bar.
	- The vertical interval between bars should be height / (no_bar + 1).

	Compile with Console.c with -pthread -D_LINUX flag
		Ex) gcc bar.c Console.c -pthread -D_LINUX


***/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <pthread.h>

#include "Console.h"

#define TRUE 1
#define FALSE 0

int repeat = TRUE;

typedef struct {
	int width, height;
	char len;
	int step;
	int x, y;
	int dx;
} ThreadParam;

void InitParam(ThreadParam *param, int width, int height);
void* runner(void *vparam);
void DrawBar(int x, int y, int len, char c);

int main(int argc, char *argv[])
{
	int no_bar = 1;

	if(argc > 1)
		no_bar =atoi(argv[1]);
	if(no_bar == 0)
		no_bar = 5;

	srand(time(NULL));
	int width = getWindowWidth();
	int height = getWindowHeight() - 1;
	
	clrscr();
	gotoxy(1, 1);
	printf("Press 'q' to quit.\n");

//  Do not enable the next line until you complete the program.
	EnableCursor(FALSE);
	
	pthread_t tid[no_bar];
	ThreadParam param[no_bar];

	for( int t = 0; t<no_bar; t++)
		InitParam(&param[t], width, height);

	//char c = 'A' + 0;
	//DrawBar(x, y, len, c);

	for( int t = 0; t<no_bar; t++)
		pthread_create(&tid[t], NULL, runner, &param[t]);

	// TO DO: Wait for 'q' key. (use getch() and while statement)
	while(getch() != 113);
	// TO DO: Stop threads by setting repeat to FALSE
	repeat = FALSE;
	// TO DO: Wait for the child threads
	for( int t = 0; t < no_bar; t++)
		pthread_join(tid[t], NULL);
	
	clrscr();
	gotoxy(1, 1);
	printf("Bye!\n");

//  Do not enable the next line until you complete the program.
	EnableCursor(TRUE);

	return 0;
}

void InitParam(ThreadParam *param, int width, int height)
{
	param->width = width;
	param->height = height;
	param->len = rand() % 6 + 5;
	param->step = rand() % 4 + 2;
	param->x = rand() % (width - param->step - 1);
	param->y = rand() % height + 1;
	param->dx = param->step;
	if(rand() % 2 == 0)
		param->dx = -(param->dx);
}
void* runner(void *vparam){
	ThreadParam *param = (ThreadParam*) vparam;
	int x = param->x;
	int y = param->y;
	char len = param->len;
	int step = param->step;
	int dx = param->dx;
	int width = param->width;
	
	char c = 'A' + 0;

	DrawBar(x,y,len,c);
	while(repeat){
		int oldx = x;

		x += dx;
		if(x <= 0){
			dx = -dx;
			x = 1 - x;
		}

		if(x > width - len + 1){
			dx = -dx;
			x = width - len + 1 - (x - width + len - 1);
		}
	
		DrawBar(oldx, y, len, ' ');
		DrawBar(x, y, len, c);

		MySleep(50);
	}
}

void DrawBar(int x, int y, int len, char c)
{
	pthread_mutex_lock(&display_mutex);
	
	gotoxy(x, y);
	for(int i = 0; i < len; i++)
		putchar(c);
	fflush(stdout);
	
	pthread_mutex_unlock(&display_mutex);
}

