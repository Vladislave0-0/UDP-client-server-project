#include "server_funcs.h"
#include "common.h"
#include "src/ListFunctions.h"

int main()
{
    struct list lst = {};
    Server_struct server = {0};

    list_ctor(&lst);

    if(ctor_server(&server) == SUCCESS)
    {
        if(Socket(&server) != SUCCESS)
        {
            dtor_server(&server);
            return ERROR_SOCKET_CREATION;
        }
        
        if(make_fork_proc(&server, &lst) != SUCCESS)
        {
            dtor_server(&server);
            return ERROR_FORK_PROC;
        }
    }


    dtor_server(&server);

    return 0;
}

// g++ server_main.cpp server_funcs.cpp src/ListFunctions.cpp Graphviz/GraphvizFunctions.cpp -o server
// g++ client_main.cpp client_funcs.cpp src/ListFunctions.cpp Graphviz/GraphvizFunctions.cpp -o client
