#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>  
#include <stdbool.h>

#include "tokens/token.h"
#include "ast/astnode.h"
#include "parser/parser.h"
#include "evaluator/evaluator.h"
#include "lexer/lex.h"






int main() {
    char s[MAX_LENGTH];
    printf("starting.....\n");
    while(1) {
        printf("##| ");
        if (fgets(s, sizeof(s), stdin) == NULL) break;
        Token *tokens =lex_input(s);
        Token *current = tokens;
        astnode *node = parse_expression(&current);
        if (node != NULL && current == NULL) {
            printf(">>> %d\n", evaluation(node));
    }   else if (node != NULL) {
            printf("parser error: unexpected token '%s'\n", current->value);
    }

    free_ast(node);
    free_tokens(tokens);



        
    



    }
    return 0;
}
