#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>  
#include <stdbool.h>
#include <error.h>

#define MAX_LENGTH 100
#define MAX_TOKENS 100





/// waht i need  helperss///////////////////////////////////////////////
const char *mathsymbols[]={"+","-","*","/","="};
int mathsymbols_count = 5;




typedef struct Token {
    int number;
    char type[20];
    char value[25];
    struct Token* next; 
} Token;

typedef struct astnode{
    char type[20];
    char value[25];

    struct astnode *left;
    struct astnode *right;

}astnode;

astnode *parse_expression(Token **current);


bool is_math_symbole( char c){
        for(int i =0;i <mathsymbols_count;i++){
            if(c == mathsymbols[i][0]){
                return true;
            }
        }
        return false;

        


    }

//moves from token to the next in one direction from head to tail
Token *advance(Token *token){
    if(token->next==NULL || token ==NULL){
        

        return NULL;
        

    }
    else{
        return token->next;
    }

}


//peeks on the next token
Token *peek(Token *token){
    if ( token ==NULL|| token->next==NULL){
        printf("nothing to peek on :)");
        return NULL;

    }
    else {
        return token->next;
    }
}


//compare the token to a value
bool match(Token **token,const char *expected_value){
    if(*token==NULL){
        return false;
    }
    if(strcmp((*token)->value,expected_value)==0){
        *token = (*token)->next;
        return true;

    }
    return false;
}










//rebuild from scratch i changed the design in my mind after some research


//function add_token 
bool add_token(Token **head,Token **tail,const char *value,const char *type,int number){
    Token *new_token = malloc(sizeof(Token));

    if(new_token==NULL){
        return false;
    }
    snprintf(new_token->value,sizeof(new_token->value),"%s",value);

    snprintf(new_token->type,sizeof(new_token->type),"%s",type);

    new_token->number = number;
    new_token->next = NULL;
    if(*head == NULL){
        *head = new_token;
        *tail =new_token;

    }else{
        (*tail)->next = new_token;
        *tail = new_token;
    }
    return true;


}




void free_tokens(Token *token) {
    while (token != NULL) {
        Token *next = token->next;
        free(token);
        token = next;
    }
}

//lexer function ////////////////////////////////////////////////////////
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


//parser part/////////////////////////////////////////
astnode *create_node(const char *type,const char *value){
    astnode *node = malloc(sizeof(astnode));

    if (node==NULL){
        return NULL;
    }

    snprintf(node->type,sizeof(node->type),"%s",type);
    snprintf(node->value,sizeof(node->value),"%s",value);

    node->left= NULL;
    node->right=NULL;

    return node;
}



void free_ast(astnode *node) {
    if (node == NULL) {
        return;
    }

    free_ast(node->left);
    free_ast(node->right);
    free(node);
}



astnode *parse_factor(Token **current){
    if (*current==NULL){
        printf("parser error: unexpected input \n");
        return NULL;

    }
    if(strcmp((*current)->type,"punct")==0 && strcmp((*current)->value,"(")==0){
        
        
        *current=advance(*current);
         astnode *node = parse_expression(current);
         if(node==NULL){
            return NULL;
         }
        if(*current ==NULL || strcmp((*current)->type,"punct")!=0 || strcmp((*current)->value,")")!=0){
            printf("parser error : expected ')'\n");
            return NULL;
        }
        *current=advance(*current);
        return node;
    }
    

    if(strcmp((*current)->type,"number")==0 ||strcmp((*current)->type,"identifier")==0 ){
        astnode *node= create_node((*current)->type,(*current)->value);
        if(node == NULL){
            return NULL;
        }
        *current=advance(*current);
        return node;

    }
    printf("parser error: expected number or identifier ,but got '%s'\n",(*current)->value);
    return NULL;
    

    
   
}

//parses divitiion and multiplication
astnode *parse_term(Token **current){
    astnode *left_node=parse_factor(current);
    if (left_node ==NULL){
        return NULL;
    }

    while (*current!=NULL && (strcmp((*current)->value,"*")==0 || strcmp((*current)->value,"/")==0  )){
        astnode *operator_node = create_node((*current)->type,(*current)->value);
        if (operator_node == NULL) {
                return NULL;
                
        }
    
        *current=advance(*current);
        astnode *right_node = parse_factor(current);
        if (right_node==NULL){
            return NULL;
            
            }
            

        operator_node->left = left_node;
        operator_node->right = right_node;

        left_node = operator_node;
        }

            

            
        
        
        
        

        

     
        return left_node;
    

    
    
    
    
    
}

//parses addition and subtraction
astnode *parse_expression(Token **current){
    astnode *left_node=parse_term(current);

    if (left_node ==NULL){
        return NULL;

    }
    while(*current!=NULL && (strcmp((*current)->value,"+")==0 || strcmp((*current)->value,"-")==0  )){
        astnode *operator_node = create_node((*current)->type,(*current)->value);
        if (operator_node == NULL) {
                return NULL;
        }
        
        *current=advance(*current);
        astnode *right_node = parse_term(current);
        if (right_node==NULL){
            return NULL;
                
                }
                

        operator_node->left = left_node;
        operator_node->right = right_node;

        left_node = operator_node;
                
        
        


    }
    
    
    
    
    return left_node;


}




//evaluation of the nodes logic and calculations

int evaluation(astnode *node){
    if(node==NULL){
        return 0;
    }
    if (strcmp(node->type,"number")==0){
        return (int)strtol(node->value,NULL,10);

    }
    int left=evaluation(node->left);
    int right=evaluation(node->right);

    if (strcmp(node->type,"operator")==0){
        if(strcmp(node->value,"+")==0){
            return  left +  right;
        }
        else if(strcmp(node->value,"-")==0){
            return left -  right;
        }
        else if (strcmp(node->value,"*")==0){
            return left *  right;
        }
        else if (strcmp(node->value, "/") == 0) {
        if (right == 0) {
            printf("evaluation error: division by zero really  :/ \n");
            return 0;
        }

        return left / right;
         }  
    
    }
    printf("evaluation error: unknown operator '%s'\n",node->value);
    return 0;
     
   
}










/*Token *lex_input(char *s) {
    const char delim[] = " \t\r\n";
    char tokens[MAX_TOKENS][MAX_LENGTH];
    int tokens_count = 0;

    char *string = strtok(s, delim);
    Token *head =NULL;
    Token *tail = NULL;

    // Process tokens inside the loop where 'string' is actually valid
    while (string != NULL) {
        if (tokens_count >= MAX_TOKENS) {
            printf("warning: maximum token limit reached\n");
            break;
        }
        
        strncpy(tokens[tokens_count], string, MAX_LENGTH - 1);
        tokens[tokens_count][MAX_LENGTH - 1] = '\0';
        
        // --- Create and evaluate token here ---
        Token* new_token = (Token*)malloc(sizeof(Token));
        if (new_token == NULL) {
            printf("memory allocation failed\n");
            return NULL;
        }
        
        strncpy(new_token->value, string, sizeof(new_token->value) - 1);
        new_token->value[sizeof(new_token->value) - 1] = '\0';
        new_token->number = tokens_count;
        new_token->next = NULL;

        // METHOD A: Using your list of strings
        
        // METHOD B: Better alternative for multi-digit numbers (like "42")
        if (isdigit((unsigned char)string[0])) {
            strcpy(new_token->type, "number");
        } 
        else if (is_math_symbole(string)){
            strcpy(new_token->type,"mathsymbole");
        }
        else {
            strcpy(new_token->type, "command");
        }

        
               
        if (head == NULL) {
            head = new_token;
            tail = new_token;
    }   else {
            tail->next = new_token;
            tail = new_token;
        }
         

        tokens_count++;
        string = strtok(NULL, delim);

        
    }

    return head;

    

    
}*/


void print_ast(astnode *node) {
    if (node == NULL) {
        return;
    }

    printf("Value: %s | Type: %s\n", node->value, node->type);

    print_ast(node->left);
    print_ast(node->right);
}




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
