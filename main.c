#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fun_stack.h"
#include "alex.h"
#include "parser.h"

#define MAXINDENTLENGHT 70     // maks długość identyfikatora

int main(int argc, char *argv[])
{
	FILE *in = argc > 1 ? fopen(argv[1], "r") : NULL;

	if(!in)
	{
		printf("Nie udalo sie otowrzyc pliku!\n");
		return -1;
	}

	//towrzenie stosu
	stos_t *stos = stos_inic();
	
	//tworzenie listy do przechowywania info
	table_t *table = inic();
	
	//analiza pliku z danymi
	analizatorSkladni(stos, argv[1], table);
	
	//wypiasnie wyniku
	show_solution(table, argv[1]);



	printf("koniec progamu\n");
	fclose(in);
	return 0;
}
