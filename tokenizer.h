#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "directoryfunctions.h"
#include "tokenizer.h"

#include<sys/stat.h>
#include <fcntl.h>  
#include<errno.h>

typedef struct Node{
	int wordLength;
	int symbolIndex;
	char *word;
	char letter;
	struct Node *next;	
} Node;

//Method Signatures: ------------------------
void printArray(char **array, int size);
Node *createWordNode(int wordLength, char *word);
Node *createLetterNode(char letter);
Node *createSymbolIndexNode(int symbolIndex);
void addToEndWordNode(Node **head, Node *target);
void addToEndLetterNode(Node **head, Node *target);
void addToEndIndexOfSymbolNode(Node **head, Node *target);
char *convertLetterLLToArrayOfLetters(Node *head);


Node *createWordNode(int wordLength, char *word){
    	Node *ptr = malloc(sizeof(Node));
    	ptr->wordLength = wordLength;
    	ptr->word = strdup(word);
    	return ptr;
}

Node *createLetterNode(char letter){
	Node *ptr = malloc(sizeof(Node));	
	ptr->letter = letter;
	return ptr;
}

Node *createSymbolIndexNode(int symbolIndex){
	Node *ptr = malloc(sizeof(Node));
	ptr->symbolIndex = symbolIndex;
	return ptr;
}

void addToEndWordNode(Node **head, Node *target){
	if(*head == NULL) {
		(*head) = target;
		return;
	}
	Node *temp = createWordNode(target->wordLength, target->word);
	Node *ptr = (*head);
	while(ptr->next != NULL){
		ptr = ptr->next;
	}
	ptr->next = temp;
}

void addToEndLetterNode(Node **head, Node *target){
	if (*head == NULL) {
		(*head) = target;
		return;
	}
	Node *temp = createLetterNode(target->letter);
	Node *ptr = (*head);
	while(ptr->next != NULL){
		ptr = ptr->next;
	}
	ptr->next = temp;
}

void addToEndIndexOfSymbolNode(Node **head, Node *target){
	if (*head == NULL) {
		(*head) = target;
		return;
	}
	Node *temp = createLetterNode(target->symbolIndex);
	Node *ptr = (*head);
	while(ptr->next != NULL){
		ptr = ptr->next;
	}
	ptr->next = temp;
}

char *convertLetterLLToArrayOfLetters(Node *head){
	Node *ptr = head;
	int numLetters = 0;
   	while(ptr != NULL){
        	numLetters++;
        	ptr = ptr->next;
    	}
    	
	char *letters = (char *) malloc(sizeof(char)*(numLetters+1));
	letters[numLetters+1] = '\0';    
	
    	ptr = head;
    
    	int i = 0;
    	while (ptr != NULL) {
        	letters[i] = ptr->letter;
        	i++;
        	ptr = ptr->next;
    	}
	return letters;
}

#endif
