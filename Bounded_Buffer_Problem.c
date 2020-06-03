#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5
#define MAX_MESSAGE 64
char buffer[BUFFER_SIZE][MAX_MESSAGE];
int in = 0, out = 0, count = 0;

pthread_mutex_t mutex;
sem_t empty, full;

int repeat = 1; // Producer and Consumer threads terminates 
                // when main() sets repeat to Zero
void DisplayBuffer();
void* produce(void* param);
void* consume(void* param);

int main(int argc, char* argv[]){
	int duration, sval;
	if(argc == 1)
		duration = 30; //default value
	else
		duration = atoi(argv[1]);
	
	//Initialize
	srand(time(NULL));
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	pthread_mutex_init(&mutex, NULL);

	DisplayBuffer();
	
	pthread_t producer, consumer;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&producer, &attr, produce, NULL);
	pthread_create(&consumer, &attr, consume, NULL);

	sleep(duration);

	repeat = 0;
	
	sem_getvalue(&full, &sval);
	if(sval == 0)
		sem_post(&full);
	sem_getvalue(&empty, &sval);
	if(sval == 0)
		sem_post(&empty);
		
	pthread_join(producer, NULL);
	pthread_join(consumer, NULL);

	pthread_mutex_destroy(&mutex);
	sem_destroy(&full);
	sem_destroy(&empty);

	printf("Bye!\n");

	return 0;
}

void* produce(void* param){
	while(repeat){
		sleep(rand()%3+1); // wait for 1~3 seconds

		int no_messages = 10;
		char *messages[64] = {"Nice to see you!", "Aal izz well!", "I love you! God loves you!", "God loves you and has a wonderful plan for your life.", "God bless you!", "You are genius!", "Cheer up!", "Everything is gonna be okay.", "You are so precious!", "Hakuna matata!"};
		char* str = messages[rand()%10];	
		printf("[Producer] Created a message \"%s\"\n", str);
		
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		//critical section
		printf("---------------------- PRODUCER -----------------------\n");
		printf("Producer is entering critical section.\n");
		printf("[Producer] \"%s\" ==> buffer\n", str);
		
		strcpy(buffer[in], str);	
		in = (in + 1) % BUFFER_SIZE;
		count++;

		DisplayBuffer();
		printf("Producer is leaving critical section.\n");
		printf("-------------------------------------------------------\n");

		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
}

void* consume(void* param){
	while(repeat){	
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		//critical section
		printf("--------------------- CONSUMER ------------------------\n");
		printf("Consumer is entering critical section.\n");

		char* str = buffer[out];
		out = (out + 1) % BUFFER_SIZE;
		count--;

		printf("[Consumer] buffer ==> \"%s\"\n", str);
		DisplayBuffer();
		printf("Consumer is leaving critical section.\n");
		printf("-------------------------------------------------------\n");

		pthread_mutex_unlock(&mutex);
		sem_post(&empty);

		printf("[Consumer] Consumed a message \"%s\"\n", str);
		sleep(rand()%3+2);
	}

}

void DisplayBuffer(){
	printf("Buffer contents:\n");
	printf("\tcount = %d, in = %d, out = %d\n", count, in, out);

	int p = out;
	for(int i = 0; i < count; i++){
		printf("\t%d] %s\n", p, buffer[p]);
		p = (p + 1) % BUFFER_SIZE;
	}
}













