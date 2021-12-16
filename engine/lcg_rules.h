//
// Created by foyjog on 13/12/2021.
//

#ifndef LIGHT_CONTAINER_GUARD_LCG_RULES_H
#define LIGHT_CONTAINER_GUARD_LCG_RULES_H

#endif //LIGHT_CONTAINER_GUARD_LCG_RULES_H
#include <stdbool.h>
#include <yaml.h>

struct lcg_rule_entity{
    struct lcg_rule_entity *next;
    char* rule_name;
    char* description;
    char* condition;
    int priority;
};


void bail(const char *msg);
void *bail_alloc(size_t size);
char *bail_strdup(const char *s);

void add_rule(struct lcg_rule_entity **rule, char *name, char *description,char* condition, int priority);
void destroy_rule(struct lcg_rule_entity **rule);