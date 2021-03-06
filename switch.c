// If the comments are hard to read because of the color,
// type ':set background=dark'

/***

	This program reads texts and switches the case of the characters (lower case <-> upper case) until the user types "quit".

	Example)
		Input a text: Welcome!
		convered text = wELCOME!
		Input a text: God bless you!
		convered text = gOD BLESS YOU!
		Input a text: quit
		Bye!

	It launches a child process and communicates with it through two ordinary pipes, one to send the original text and the other to receive the converted text.

	Complete the program using ORDINARY PIPES by following the instructions.
	DO NOT use other IPC such as shared memory or message queue.

	The main creates two pipes and a child process.
	
	Then, the parent process repeats the followings until the user types "quit".
		Read a text line from the user.
		Send the text to the child through pipe.
		Receive and display the converted text.

	The child process repeats the followings until it receives "quit" from the parent. 
		Read a text line from the parent through one pipe.
		Convert all upper case characters to the corresponding lower case characters and vice versa.
		Write the converted text to the parent through the other pipe.

	Hint) To read a text line from the user, use the following code.
			printf("Input a text: ");
			fgets(src, BUFFER_SIZE - 1, stdin);	
			int len = strlen(src) - 1;
			src[len] = 0;				// trim '\n'

	Note! Before the parent terminates, there the child processing MUST terminate.
		You can check whether the child is still running by check currently running processes by 'ps -al'.

***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define TRUE 1
#define FALSE 0

#define READ_END 0
#define WRITE_END 1

int repeat = TRUE;

#define BUFFER_SIZE 256

void SwitchCase(int in[], int out[]);

char org_buf[BUFFER_SIZE];
char cvt_buf[BUFFER_SIZE];

int main()
{
	int in[2];		// pipe from parent to child
	int out[2];		// pipe from child to parent

	//	TO DO: Create two ordinary pipes
	if(pipe(in)==-1){
		fprintf(stderr, "Pipe failed");
		return 1 ;
	}

	if(pipe(out)==-1){
		fprintf(stderr, "Pipe failed");
		return 1 ;
	}

	//  TO DO: Create child process
	pid_t pid = fork();
	
	if(pid < 0){
		fprintf(stderr, "Fork Failed");
		return 1;
	}

	// On parent process,
		// Repeats the followings until the user types "quit".
			// Read a text line from the user.
			// Send the text to the child through pipe.
			// Receive and display the converted text.

		// Wait for the child to terminate
		// Deallocate pipes
		// Print a good-bye message
	else if(pid > 0){
		close(in[READ_END]);
		close(out[WRITE_END]);
		
		while(repeat){
			
			printf("Input a text : ");
			fgets(org_buf, BUFFER_SIZE - 1, stdin);
			int len = strlen(org_buf) - 1;
			org_buf[len] = 0;	//trim'\n'
			
			if(strcmp(org_buf,"quit") == 0)
				repeat = FALSE;

			write(in[WRITE_END], org_buf, strlen(org_buf)+1);
			
			if(repeat == FALSE)
				break;

			read(out[READ_END], cvt_buf, BUFFER_SIZE);
			printf("converted text = %s\n", cvt_buf);
			
		}
		
		wait(NULL);

		close(in[WRITE_END]);
		close(out[READ_END]);
		
		printf("Bye!\n");
	}

	// On child process call SwitchCase(in, out);
	else{
		SwitchCase(in, out);
		exit(0);
	}

	return 0;
}

void SwitchCase(int in[], int out[])
{
	char buffer[BUFFER_SIZE];

	// TO DO: Implement the following algorithm
	// Repeats the followings until it receives "quit" from the parent. 
		//	Receive a text line from the parent through pipe in.
		//	Convert all upper case characters to the corresponding lower case characters and vice versa.
		// 		You may use isupper(), islower(), toupper(), tolower() functions.
		//		But DO NOT use other IPC such as shared memory or message queue.
		//	Send the converted text to the parent through pipe out.

	// TO DO: deallocate pipes;
	close(in[WRITE_END]);
	close(out[READ_END]);

	while(1){
		
		read(in[READ_END], buffer, BUFFER_SIZE);
		
		if(strcmp(buffer,"quit") == 0)
			break;

		for(int i = 0; i < strlen(buffer); i++){
			if('A' <= buffer[i] && buffer[i] <= 'Z')
				cvt_buf[i] = buffer[i] + 32;
			
			else if('a' <= buffer[i] && buffer[i] <= 'z')
				cvt_buf[i] = buffer[i] - 32;
			else
				cvt_buf[i] = buffer[i];
		}

		cvt_buf[strlen(buffer)] = '\0';

		write(out[WRITE_END], cvt_buf, strlen(cvt_buf)+1);
	}
	
	close(in[READ_END]);
	close(out[WRITE_END]);
}



















