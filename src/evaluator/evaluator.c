#include <stdbool.h>
#include "evaluator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int evaluation(astnode *node){
    if(node==NULL){
        return 0;
    }
    if (strcmp(node->type,"number")==0){
        return (int)strtol(node->value,NULL,10);

    }
    
    
    int left=evaluation(node->left);
    int right=evaluation(node->right);
    if (strcmp(node->type,"unary")==0){
        return -evaluation(node->right);
    }

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
