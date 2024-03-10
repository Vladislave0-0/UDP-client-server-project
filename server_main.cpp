#include "server_funcs.h"
#include "common.h"

int main()
{
    Server_struct server = {0};

    if(ctor_server(&server) == SUCCESS)
    {
        if(Socket(&server) != SUCCESS)
        {
            dtor_server(&server);
            return ERROR_SOCKET_CREATION;
        }
        
        if(make_fork_proc(&server) != SUCCESS)
        {
            dtor_server(&server);
            return ERROR_FORK_PROC;
        }
    }


    dtor_server(&server);

    return 0;
}