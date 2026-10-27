#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>  
#include <stdbool.h>

#include "../tokens/token.h"




#ifndef LEX_H
#define LEX_H

#define MAX_LENGTH 100
#define MAX_TOKENS 100

Token *lex_input(char *s);







#endif
