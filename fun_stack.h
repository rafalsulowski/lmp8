#ifndef _FUN_STACK_H_
#define _FUN_STACK_H_


typedef struct stos{
    char *fun_name;     // nazwa funkcji
    int bil;            // bilans nawiasów
    struct stos *next; // nastepna struktura
} stos_t;

//deklaracje - prototypy
stos_t* stos_inic(void); //tworzy stos
stos_t* put_on_fun_stack(stos_t*, int, char*); // dodaje pare na stos
stos_t* usun_pare(stos_t*); // usuwa pare bilans i nazwa ze stosu
int 	top_of_fun_stack(stos_t*); // pokazuje jaki jest bilans na gorze stosu
char*	get_from_fun_stack(stos_t*); // pokazuje jaka nazwa funkcji jest na gorze stosu


#endif

