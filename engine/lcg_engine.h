//
// Created by foyjog on 13/12/2021.
//

#ifndef LIGHT_CONTAINER_GUARD_LCG_ENGINE_H
#define LIGHT_CONTAINER_GUARD_LCG_ENGINE_H

#endif //LIGHT_CONTAINER_GUARD_LCG_ENGINE_H
#include "yaml_parse.h"



struct k8s_event{
    struct pod_state* pod;
};

struct pod_state{
    int privileged;
    char* mount;
};

