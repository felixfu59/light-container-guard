//
// Created by foyjog on 13/12/2021.
//

#include "lcg_rules.h"

void
bail(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

/* Helper to allocate memory or bail. */
void *
bail_alloc(size_t size)
{
    void *p = calloc(1, size);
    if (!p) {
        bail("out of memory");
    }
    return p;
}

/* Helper to copy a string or bail. */
char *
bail_strdup(const char *s)
{
    char *c = strdup(s ? s : "");
    if (!c) {
        bail("out of memory");
    }
    return c;
}

void
add_rule(struct lcg_rule_entity **rule, char *rule_name, char *description,char* condition, int priority)
{
    /* Create fruit object. */
    struct lcg_rule_entity *r = bail_alloc(sizeof(struct lcg_rule_entity));
    r->rule_name = bail_strdup(rule_name);
    r->description = bail_strdup(description);
    r->condition =  bail_strdup(condition);;
    r->priority = priority;

    /* Append to list. */
    if (!*rule) {
        *rule = r;
    } else {
        struct lcg_rule_entity *tail = *rule;
        while (tail->next) {
            tail = tail->next;
        }
        tail->next = r;
    }
}

void
destroy_rule(struct lcg_rule_entity **rule)
{
    for (struct lcg_rule_entity *f = *rule; f; f = *rule) {
        *rule = f->next;
        free(f->rule_name);
        free(f->description);
        free(f->condition);
        free(f);
    }
}


