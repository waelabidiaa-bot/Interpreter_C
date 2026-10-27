#include <stdio.h>
#include <string.h>

#include "var.h"

variable vars[MAX_VARS];
int var_count = 0;

void set_var(const char *name, int value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            vars[i].value = value;
            return;
        }
    }

    if (var_count >= MAX_VARS) {
        printf("Error: max variables reached\n");
        return;
    }

    snprintf(vars[var_count].name, sizeof(vars[var_count].name), "%s", name);
    vars[var_count].value = value;
    var_count++;
}

int get_var(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            return vars[i].value;
        }
    }

    printf("variable error: '%s' is not defined\n", name);
    return 0;
}
