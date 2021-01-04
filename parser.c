#include "parser.h"
#include "fun_stack.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // exit - ale exit trzeba kiedyś usunąć i nie będzie to potrzebne

static FILE *in = NULL;


table_t* inic(void)
{
	table_t *tmp = (table_t*) malloc(sizeof(tmp));
	tmp->fun_name = (char*) malloc(sizeof(char) * (MAXINDENTLENGHT + 1));
	
	for(int i = 0; i < MAX_CALL; i++)
		tmp->calltab[i] = -1;
	
	tmp->def_line = -1;
	tmp->def2_line = -1;
	tmp->prot_line = -1;
	
	tmp->next = NULL;
	
	return tmp;
}

table_t* push(table_t *head, char* funname, int line, char type)
{
	int val = 0;
	
	//sprawdzanie czy istnieje juz taki element z taka nazwa funkcji
	table_t *it = head;
	for( ; it != NULL; it = it->next)
	{		
		if(!strcmp(it->fun_name, funname))
		{
			val = 1;
			break;
		}
	}
	
	if(val == 1)
	{
		switch(type)
		{
			case 'd':
			{
				it->def_line = line;
			}
			break;
				
			case 'p':
			{
				it->prot_line = line;
			}
			break;
			
			case 'c':
			{
				for(int i = 0; i < MAX_CALL; i++)
				{
					if(it->calltab[i] >= 0)
						continue;
					else
					{
						it->calltab[i] = line;
						break;
					}
				}	
			}
			break;
			
			default:
				break;
		}
		
		return head;
	}
	else 
	{
		table_t *tmp = (table_t*) malloc(sizeof(tmp));
		tmp->fun_name = (char*) malloc(sizeof(char) * (MAXINDENTLENGHT + 1));
		
		for(int i = 0; i < MAX_CALL; i++)
			tmp->calltab[i] = -1;
		
		tmp->def_line = -1;
		tmp->def2_line = -1;
		tmp->prot_line = -1;
		
		switch(type)
		{
			case 'd':
			{
				tmp->def_line = line;
			}
			break;
			
			case 'p':
			{
				tmp->prot_line = line;
			}
			break;
			
			case 'c':
			{
				for(int i = 0; i < MAX_CALL; i++)
				{
					if(tmp->calltab[i] >= 0)
						continue;
					else
					{
						tmp->calltab[i] = line;
						break;
					}
				}				
			}
			break;
			
			default:
				break;		
		}
		
		strcpy(tmp->fun_name,funname);
		
		while(head->next != NULL)
			head = head->next;
		
		head->next = tmp;
		tmp->next = NULL;
		
		return head;
	}
}

table_t* modifi_def_line(table_t* head, int line, char* fn)
{
	int val = 0;
	
	//ustawiamy iterator na element z funkcja ktorej konczy sie deklaracja:
	table_t *it = head;
	for( ; it != NULL; it = it->next)
	{
		if(!strcmp(it->fun_name, fn))
		{
			val = 1;
			break;
		}
	}
	
	if(val == 0)
	{
		printf("Nie znalzeiono takiej funkcji, cos poszlo nie tak!\n");
		return head;		
	}
	else
	{
		it->def2_line = line;
	}		
	
	return head;
}

void show_solution(table_t *head, char* file_name)
{
	int zm = 0;	
	
	table_t *it = NULL;
	for(it = head->next; it != NULL; it = it->next)
	{
		printf("\nFunkcja o nazwie \"%s\", w pliku \"%s\" ma:\n", it->fun_name, file_name);
		
		if(it->prot_line >= 0)
			printf("Prototyp w lini: %d\n", it->prot_line);
		else
			printf("Prototyp w lini: ---\n");
			
		if(it->def_line >= 0)
			printf("Definicje w liniach od: %d do %d\n", it->def_line, it->def2_line);
		else
			printf("Definicje w lini: ---\n");
		
		if(it->calltab[0] >= 0 && it->calltab[0] < 1000)
		{
			printf("Wywolanie w lini: %d, ", it->calltab[0]);
			zm = 1;
		}
		else
		{
			printf("Wywolanie w lini: ---\n");
			zm = 0;
		}
		
		if(zm == 1)
		{
			for(int i = 1; i < MAX_CALL; i++)
			{
				//mniejeze od 1000 raczej nie bedzie wiekszyuhgc plikow
				//a bez tego warunku wypisuje kosmiczne liczby niewaidomo skad
				if(it->calltab[i] >= 0 && it->calltab[i] < 1000)
					printf("%d, ", it->calltab[i]);
				else
					break;
			}
			zm = 0;
		}
		
		printf("\n");	
	}
}



void store_add_def(char* fun_name, int line, table_t* head){
	
	head = push(head, fun_name, line, 'd');
}

void store_add_proto(char* fun_name, int line, table_t* head){
	
	head = push(head, fun_name, line, 'p');
}

void store_add_call(char* fun_name, int line, table_t* head){
	
	head = push(head, fun_name, line, 'c');
}



void analizatorSkladni (stos_t *s, char *inpname, table_t *head)
{ 
	in = fopen(inpname, "r");
	if(!in)
	{
		printf("Blad otwarcia pliku\n");
		exit(EXIT_FAILURE);
	}
	
	int zm = 0, zm2 = 0;
	char *fn = (char*) malloc((MAXINDENTLENGHT + 1) * sizeof(char*));
	int nbra = 0;   // bilans nawiasów klamrowych {}
	int npar = 0;   // bilans nawiasów zwykłych ()
	
	alex_init4file (in);          // ustaw analizator leksykalny, aby czytał in
	lexem_t lex;
	lex = alex_nextLexem ();      // pobierz następny leksem
	
	
	while (lex != EOFILE)
	{
		switch (lex) 
		{
			case IDENT:
			{
					char *iname = alex_ident ();   // zapamiętaj identyfikator i patrz co dalej
					lexem_t nlex = alex_nextLexem ();
					if (nlex == OPEPAR) 
					{   // nawias otwierający - to zapewne funkcja
						npar++;
						s = put_on_fun_stack (s, npar, iname);      // odłóż na stos funkcji
						   // stos f. jest niezbędny, aby poprawnie obsłużyć sytuacje typu
						   // f1( 5, f2( a ), f3( b ) )
					}
					else 
					{                  // nie nawias, czyli nie funkcja
					   lex = nlex;
					   continue;
				    }
			}
			break;
			
			case OPEPAR:
				npar++;
				break;
			
			case CLOPAR:
			{              // zamykający nawias - to może być koniec prototypu, nagłówka albo wywołania
				if (top_of_fun_stack(s) == npar)
				{       // sprawdzamy, czy liczba nawiasów bilansuje się z wierzchołkiem stosu funkcji
					// jeśli tak, to właśnie wczytany nawias jest domknięciem nawiasu otwartego
					// za identyfikatorem znajdującym się na wierzchołku stosu
					lexem_t nlex = alex_nextLexem ();     // bierzemy nast leksem
					
					if(nlex == CLOPAR)
						zm2 = 1;					
					
					if (nlex == OPEBRA)   // nast. leksem to klamra a więc mamy do czynienia z def. funkcji
					{
						strcpy(fn,get_from_fun_stack(s));
						store_add_def (fn, alex_getLN (), head);
						zm = 1;
						nbra++;
					}
					else if (nbra == 0)   // nast. leksem to nie { i jesteśmy poza blokami - to musi być prototyp
						store_add_proto (get_from_fun_stack(s), alex_getLN () + 1, head);
					else                  // nast. leksem to nie { i jesteśmy wewnątrz bloku - to zapewne wywołanie
						store_add_call (get_from_fun_stack(s), alex_getLN () + 1, head);
				}
				npar--;
			}
			break;
			
			case OPEBRA:
				nbra++;
				break;
			
			case CLOBRA:
				nbra--;
				
				if(zm == 1) //czyli zakonczyla sie deficnicja
				{
					head = modifi_def_line(head, alex_getLN(), fn);
					zm = 0;
				}
				
				break;
			
			case ERROR:
			{
				fprintf (stderr, "\nBUUUUUUUUUUUUUUUUUUUUUU!\n"
				"W pliku %s (linia %d) są błędy składni.\n"
				"Kończę!\n\n", inpname, alex_getLN());
				exit (EXIT_FAILURE);         // to nie jest najlepsze, ale jest proste ;-)
			}
			break;
			
			default:
				break;
		}
		
		if(zm2 == 1)
		{
			zm2 = 0;
			continue;
		}
		else
			lex = alex_nextLexem ();
		
	}
}
