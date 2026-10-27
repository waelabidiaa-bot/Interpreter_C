#ifndef VAR_H
#define VAR_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_VARS 200

typedef struct {
    char name[25];
    int value;
} variable;

extern variable vars[MAX_VARS];
extern int var_count;

void set_var(const char *name, int value);
int get_var(const char *name);

#endif