#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct zNode {
	struct zNode* left;
	struct zNode* right;
	char letter;
	int freq;
	char* zcode;
}znode;

typedef struct QNode { 
	struct zNode* key; 
	struct QNode* next;
}qnode;

void huffman_code(znode* a, int n, char* code);
#define root -1
znode* dequeue(); // deQueue
qnode* pri_Q = NULL; // Priority Queue
qnode* enqueue(znode* r); // enQueue

int main(void) {
	char text[] = "HGU CSEE is a higher educational institution that cultivates excellent and honest professionals who will serve God and people by playing a role as salt and light in the darkening world. A CSEE student should strive to develop expertise in IT-related fields based on the unshakable integrity of the Christian spirit exemplified by love and righteousness. This guideline aims to clarify the standard of honesty, sincerity and responsibility the students should keep in order that they may overcome temptations they encounter during their studies and develop into trustworthy and capable professionals.";
	printf("%s\n", text);
	printf("The length of the give text is %ld\n", strlen(text));

	int i, j, index = 0;
	char* letter = (char*)calloc(128,sizeof(char));
	int* freq = (int*)calloc(128,sizeof(int)); 

	for(i = 0; i < strlen(text); i++){
		for(j = 0; letter[j]!='\0'; j++){
			if(text[i] == letter[j]){ 
				freq[j]++;
				index = 1;
				break;
			}
		}
		if(index == 0){ 
			letter[j] = text[i];
			freq[j] = 1;
		}
		index = 0;
	}
	int letter_count = strlen(letter);

	int n = 0;
	while(1){
		znode* r = (znode*)malloc(sizeof(znode));
		if(n == letter_count) break;

		r->letter = letter[n];
		r->freq = freq[n];
		r->left = NULL;
		r->right = NULL;
		r->zcode = NULL;
		enqueue(r);
		n++;
	}

	znode* first; znode* second;
	znode* huffman;

	for(i = 0; i < letter_count - 1; i++){
		first = dequeue();
		second = dequeue();

		huffman = (znode*)malloc(sizeof(znode));
		huffman -> left = first;
		huffman -> right = second;
		huffman -> freq = first->freq + second->freq;
		huffman -> letter = '*';
		huffman -> zcode = NULL;
		enqueue(huffman);
	}
	huffman = dequeue();

	// print your optimal Huffman code
	char* code = (char*)malloc(sizeof(char));
	printf("\nLetter   |code\n");
	printf("---------------------\n");
	huffman_code(huffman, root, code);
	printf("---------------------\n");

	return 0;
}

qnode* enqueue(znode* r){
	qnode* new = NULL;
	qnode* head = NULL;
	new = (qnode*)malloc(sizeof(qnode));
	new -> key = r;
	new -> next = NULL;

	if(pri_Q == NULL)
		pri_Q = new;
	else if(pri_Q->key->freq > new->key->freq){
		new->next = pri_Q;
		pri_Q = new;
	}
	else{
		head = pri_Q;
		while(head->next != NULL){
			if(head->next->key->freq <= new->key->freq) head = head->next;
			else{
				new->next = head->next;
				head->next = new;
				break;
			}
		}
		if(head->next == NULL) head->next = new;
	}
	return pri_Q;
}

znode* dequeue(){
	znode* firstnode = NULL;
	firstnode = pri_Q -> key;
	pri_Q = pri_Q -> next;

	return firstnode;
}

void huffman_code(znode* huffman, int i, char* code){
	if(huffman){
		i++;

		code[i] = '0';
		huffman_code(huffman->left, i, code);
		code[i] = '1';
		huffman_code(huffman->right, i, code);
		code[i] = '\0';
		if(huffman->left == NULL || huffman->right == NULL){
			huffman->zcode = code;
			printf("%c        | %s\n", huffman->letter, huffman->zcode);
		}
	}
}

