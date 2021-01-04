#ifndef PARSER_H
#define PARSER_H

#include "alex.h"       // analizator leksykalny
#include "fun_stack.h"  // stos funkcji

#define MAX_CALL 10
#define MAXINDENTLENGHT 70     // maks długość identyfikatora

typedef struct list{
	char *fun_name;
	int prot_line;
	int def_line;
	int def2_line;
	int calltab[MAX_CALL];
	
	struct list *next;
} table_t;

//dane przechowywane sa w liscie jednokierunkowej
//ponizej sa funkcje do obslugiwanani listy na potrzeby programu:

table_t* push(table_t*, char*, int, char);		//dodaje element do listy
table_t* inic(void);	//inicjuje liste
table_t* modifi_def_line(table_t*, int, char*);		//dodaje koncowa linie definicji fuujncki
void show_solution(table_t*, char*);	//wypisuje na ekran wynik pracy programu

void store_add_def(char*, int, table_t*);
void store_add_proto(char*, int, table_t*);
void store_add_call(char*, int, table_t*);

void analizatorSkladni (stos_t* , char*, table_t*);





#endif