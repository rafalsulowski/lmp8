#include "alex.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>

static int  ln= 0;
static char ident[256];
static FILE *ci= NULL;
static int c;

void    alex_init4file( FILE *in ) {
	ln= 0;
	ci= in;
}

int isKeyword(char *str){
	char *keyword = "auto, break, case, char, const, continue, default, do, double, else, enum, extern, float, for, goto, if, inline, int, long, register, restrict, return, short, signed, sizeof, static, struct, switch, typedef, union, unsigned, void, volatile, while, _Alignas, _ALignof, _Atomic, _Bool, _Complex, _Generic, _Imaginary, _Noreturn, _Static_assert, _Thread_local";

	if(strstr(keyword, str))
		return 1;
	else
		return 0;
}

int getC(void)
{
	return c;
}

lexem_t alex_nextLexem( void ) {
	//printf("przed while w alex_next_lexem\n");
	while( (c = fgetc(ci)) != EOF ) {
		//printf("po while w alex_next_lexem\n");
		if( c == '\n' )
			ln++;
		if( isspace( c ) )
			continue;
		else if( c == '(' )
			return OPEPAR;
		else if( c == ')' )
			return CLOPAR;
		else if( c == '{' )
			return OPEBRA;
		else if( c == '}' )
			return CLOBRA;			
		else if( isalpha( c ) ) {
			int i= 1;
			ident[0] = c;
			while( isalnum( c= fgetc(ci) ) )
				ident[i++] = c;
			ident[i] = '\0';
			return isKeyword(ident) ? OTHER : IDENT; //OTHER - jezeli wyraz ident jest slowem specjalnym (np. if)
		} 								//IDENT - jezeli jest nazwa funkcji lub zmiennej

		else if( c == '"' ) 
		{
			/* Uwaga: tu trzeba jeszcze poprawic obsluge nowej linii w trakcie napisu
			   i \\ w napisie
			   */
			int cp = c;
			while( (c= fgetc(ci)) != EOF && c != '"' && cp == '\\' ) 
			{
				cp = c;
			}
			return c==EOF ? EOFILE : OTHER; 
		} 

		else if( c == '/' )
		{
			/* moze byc komentarz */
		} 
		if( isdigit( c ) || c == '.' ) 
		{
			/* liczba */
		} 
		else 
		{
			return OTHER;
		}
	} 
	
	return EOFILE;
}

char *  alex_ident( void ) {
	return ident;
}

int     alex_getLN() {
	return ln;
}

