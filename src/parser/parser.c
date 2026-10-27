#include <stdbool.h>
#include "../ast/astnode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../variables/var.h"

astnode *parse_expression(Token **current);

astnode *parse_factor(Token **current){
    if (*current==NULL){
        printf("parser error: expected input \n");
        return NULL;

    }
    if(strcmp((*current)->value,"-")==0){
        *current = advance(*current);

        astnode *operand = parse_factor(current);
        if (operand == NULL){
            return NULL;
        }

        astnode *node = create_node("unary","-");
        if (node == NULL){
            return NULL;

        }
        node->right = operand;
        return node;
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
    if (strcmp((*current)->value,"let")==0)
    {
        astnode *let_node=create_node("let","let");
        if (let_node==NULL)
        {
            return NULL;
        }

        *current=advance(*current);

        if(*current == NULL || strcmp((*current)->type,"identifier")!=0){
            printf("Error: expected variable name\n");
            return NULL;
        }

        astnode *var_node=create_node("var",(*current)->value);
        if(var_node==NULL){
            return NULL;
        }
        *current=advance(*current);

        if (*current == NULL || strcmp((*current)->value,"=")!=0){
            printf("Error: expected '='\n");
            return NULL;
        }

        *current = advance(*current);
        astnode *value_node=parse_expression(current);
        if(value_node==NULL){
            return NULL;
        }

        let_node->right=value_node;
        let_node->left=var_node;
        return let_node;
    }

    if(strcmp((*current)->type,"identifier")==0){
        Token *next = advance(*current);
        if (next != NULL && strcmp(next->value,"=")==0){
            astnode *var_node = create_node("var",(*current)->value);
            if(var_node==NULL){
                return NULL;
            }

            *current = next;
            *current = advance(*current);
            astnode *value_node = parse_expression(current);
            if(value_node==NULL){
                return NULL;
            }

            astnode *assign_node = create_node("assign","=");
            if (assign_node == NULL){
                return NULL;
            }

            assign_node->left = var_node;
            assign_node->right = value_node;
            return assign_node;
        }

        astnode *node = create_node((*current)->type,(*current)->value);
        if(node == NULL){
            return NULL;
        }
        *current=advance(*current);
        return node;
    }

    if(strcmp((*current)->type,"number")==0){
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




