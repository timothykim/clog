#include <stdlib.h>
#include <stdio.h>

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/server.h>
#include <xmlrpc-c/server_cgi.h>

#include "config.h"


static xmlrpc_value *
sample_add(xmlrpc_env *   const env,
           xmlrpc_value * const param_array,
           void *         const user_data) {

    xmlrpc_int32 x, y, z;

    xmlrpc_decompose_value(env, param_array, "(ii)", &x, &y);
    /*
    if (env->fault_occured)
        return NULL;
        */

    z = x + y;

    return xmlrpc_build_value(env, "i", z);
}


int
main() {
    xmlrpc_registry * registryP;
    xmlrpc_env env;

    xmlrpc_env_init(&env);
    registryP = xmlrpc_registry_new(&env);
    xmlrpc_registry_add_method(
        &env, registryP, NULL, "sample.add", &sample_add, NULL);
    xmlrpc_server_cgi_process_call(registryP);
    xmlrpc_registry_free(registryP);
    return 0;
}
