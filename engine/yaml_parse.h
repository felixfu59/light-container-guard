//
// Created by foyjog on 14/12/2021.
//

#ifndef LIGHT_CONTAINER_GUARD_YAML_PARSE_H
#define LIGHT_CONTAINER_GUARD_YAML_PARSE_H

#endif //LIGHT_CONTAINER_GUARD_YAML_PARSE_H
#include "lcg_rules.h"

/* yaml_* functions return 1 on success and 0 on failure. */
enum status {
    SUCCESS = 1,
    FAILURE = 0
};

/* Our example parser states. */
enum state {
    STATE_START,    /* start state */
    STATE_STREAM,   /* start/end stream */
    STATE_DOCUMENT, /* start/end document */
    STATE_SECTION,  /* top level */

    STATE_RLIST,    /* fruit list */
    STATE_RVALUES,  /* fruit key-value pairs */
    STATE_RKEY,     /* fruit key */
    STATE_RNAME,  /* fruit key-value pairs */
    STATE_RDESCIPTION,     /* fruit key */
    STATE_RCONDITION,    /* fruit name value */
    STATE_RPRIORITY,   /* fruit color value */

    STATE_STOP      /* end state */
};
/* Our application parser state data. */
struct parser_state {
    enum state state;      /* The current parse state */
    struct lcg_rule_entity f;        /* Fruit data elements. */
    struct lcg_rule_entity *flist;   /* List of 'fruit' objects. */
};

int consume_event(struct parser_state *s, yaml_event_t *event);