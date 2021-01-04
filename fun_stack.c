#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fun_stack.h"

stos_t* put_on_fun_stack(stos_t* head, int b, char* fn){
	stos_t *tmp = (stos_t*) malloc(sizeof(stos_t*));
	tmp->fun_name = (char*) malloc(sizeof(char) * (strlen(fn) + 1));

	tmp->bil = b;
	strcpy(tmp->fun_name, fn);
	tmp->next = head;
	
	return tmp;
}

char* get_from_fun_stack(stos_t* head){
	return head->fun_name;	
}

int top_of_fun_stack(stos_t* head){
	return head->bil;
}

stos_t* usun_pare(stos_t* head){
	stos_t *tmp = head;

	free(tmp->fun_name);
	free(tmp);

	return head->next;
}

stos_t* stos_inic(void){
	stos_t *tmp = (stos_t*) malloc(sizeof tmp);

	tmp->bil = 0;
	tmp->fun_name = NULL;
	tmp->next = NULL;

	return tmp;	
}








