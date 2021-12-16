//
// Created by foyjog on 13/12/2021.
//

#include <lualib.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <string.h>
#include "lcg_engine.h"


static struct parser_state global_rules_state;
static lua_State *L;
int load_rules(char* file_name){
    int code;
    enum status status;
    yaml_parser_t parser;
    yaml_event_t event;

    FILE *fp_rules = fopen(file_name, "r");
    if(fp_rules == NULL){
        printf("open rules failed! %s\n",file_name);
    }
    memset(&global_rules_state, 0, sizeof(global_rules_state));
    global_rules_state.state = STATE_START;
    yaml_parser_initialize(&parser);
    yaml_parser_set_input_file(&parser, fp_rules);
    do {
        status = yaml_parser_parse(&parser, &event);
        if (status == FAILURE) {
            fprintf(stderr, "yaml_parser_parse error\n");
            code = EXIT_FAILURE;
            goto done;
        }
        status = consume_event(&global_rules_state, &event);
        if (status == FAILURE) {
            fprintf(stderr, "consume_event error\n");
            code = EXIT_FAILURE;
            goto done;
        }
        yaml_event_delete(&event);
    } while (global_rules_state.state != STATE_STOP);

    code = EXIT_SUCCESS;
    return code;

    done:
        destroy_rule(&global_rules_state.flist);
        yaml_parser_delete(&parser);
    return code;
}

int fire_rule(struct k8s_event* factor, struct lcg_rule_entity *f){

    char pod_str[200];
    char k_event_str[100];
    char *condition = (char*) malloc(strlen(f->condition)+20);
    char *lua_script = (char*) malloc(310 + strlen(condition));

    sprintf(pod_str, "ps = {privileged=%d, mount=\"%s\"}",factor->pod->privileged,factor->pod->mount);
    sprintf(k_event_str,"k_event = {pod = ps}");
    sprintf(condition,"function fire()\n\treturn %s\nend",f->condition);
    sprintf(lua_script,"%s\n%s\n%s\n",pod_str,k_event_str,condition);

    //printf("%s",lua_script);
    if (luaL_dostring(L, lua_script) != 0) {
        printf("cannot run lua script!\n");
        exit(-1);
    }
    lua_getglobal(L, "fire");
    if (lua_pcall(L, 0, 1, 0) != 0) {
        printf("call func failed, got error: \n");
        lua_pop(L, 1);
        return 0;
    }
    if(lua_isboolean(L,-1)){
        return lua_toboolean(L,-1);
    }

    return 0;
}

void fire_all_rules(struct k8s_event* factor){

    for (struct lcg_rule_entity *f = global_rules_state.flist; f; f = f->next) {
        if(fire_rule(factor,f)){
            printf("Alert!! %s\n", f->description);
        }
    }
}

struct k8s_event* get_k8s_event_test(){
    struct k8s_event* k_event = ( struct k8s_event* ) malloc(sizeof (struct k8s_event));
    k_event->pod = ( struct pod_state* ) malloc(sizeof (struct pod_state));
    k_event->pod->privileged = 1;
    k_event->pod->mount = "/etc/hosts";
    return k_event;
}

int main(){
    //load rules
    load_rules("rules/k8s_rules.yaml");

    L = luaL_newstate();
    if (L == NULL) {
        printf("create state failed!");
        exit(-1);
    }
    luaL_openlibs(L);

    struct k8s_event* factor = get_k8s_event_test();

    fire_all_rules(factor);

}