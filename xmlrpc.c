#include <xmlrpc.h>
#include <xmlrpc_cgi.h>

xmlrpc_value *
sumAndDifference (xmlrpc_env *env, xmlrpc_value *param_array, void *user_data)
{
    xmlrpc_int32 x, y;

    /* Parse our argument array. */
    xmlrpc_parse_value(env, param_array, "(ii)", &x, &y);
    if (env->fault_occurred)
        return NULL;

    /* Return our result. */
    return xmlrpc_build_value(env, "{s:i,s:i}",
                              "sum", x + y,
                              "difference", x - y);
}

int main (int argc, char **argv)
{
    /* Set up our CGI library. */
    xmlrpc_cgi_init(XMLRPC_CGI_NO_FLAGS);

    /* Install our only method (with a method signature and a help string). */
    xmlrpc_cgi_add_method_w_doc("sample.sumAndDifference",
                                &sumAndDifference, NULL,
                                "S:ii", "Add and subtract two integers.");

    /* Call the appropriate method. */
    xmlrpc_cgi_process_call();

    /* Clean up our CGI library. */
    xmlrpc_cgi_cleanup();
}

