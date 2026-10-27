#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>  
#include <stdbool.h>

#include "lex.h"



const char *mathsymbols[]={"+","-","*","/","="};
int mathsymbols_count = 5;











bool is_math_symbole( char c){
        for(int i =0;i <mathsymbols_count;i++){
            if(c == mathsymbols[i][0]){
                return true;
            }
        }
        return false;

        

}













Token *lex_input(char *s) {
    Token *head = NULL;
    Token *tail = NULL;

    int i = 0;
    int token_count = 0;

    while (s[i] != '\0') {

        if (isspace((unsigned char)s[i])) {
            i++;
            continue;
        }

        char value[MAX_LENGTH];
        int j = 0;

        // 1. Numbers
        if (isdigit((unsigned char)s[i])) {

            while (isdigit((unsigned char)s[i])) {
                value[j++] = s[i++];
            }

            value[j] = '\0';

            if (!add_token(&head, &tail, value, "number", token_count++)) {
                printf("error: tokenisation failed\n");
                break;
            }
        }

        // 2. Identifiers
        else if (isalpha((unsigned char)s[i]) || s[i] == '_') {

            while (isalnum((unsigned char)s[i]) || s[i] == '_') {
                value[j++] = s[i++];
            }

            value[j] = '\0';

            if (!add_token(&head, &tail, value, "identifier", token_count++)) {
                printf("error: tokenisation failed\n");
                break;
            }
        }

        // 3. Mathematical operators
        else if (is_math_symbole(s[i])) {

            value[0] = s[i];
            value[1] = '\0';
            i++;

            if (!add_token(&head, &tail, value, "operator", token_count++)) {
                printf("error: tokenisation failed\n");
                break;
            }
        }

        // 4. Other punctuation: (, ), etc.
        else if (ispunct((unsigned char)s[i])) {

            value[0] = s[i];
            value[1] = '\0';
            i++;

            if (!add_token(&head, &tail, value, "punct", token_count++)) {
                printf("error: tokenisation failed\n");
                break;
            }
        }

        // 5. Invalid characters
        else {
            printf("lexer error: unexpected character '%c'\n", s[i]);
            break;
        }

        if (token_count >= MAX_TOKENS) {
            printf("warning: maximum tokens reached\n");
            break;
        }
    }

    return head;
}

