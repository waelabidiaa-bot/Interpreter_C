#include <stdbool.h>
#include "../ast/astnode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef PARSER_H
#define PARSER_H

astnode *parse_factor(Token **current);
astnode *parse_term(Token **current);
astnode *parse_expression(Token **current);


#endif