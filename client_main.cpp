#include "client_funcs.h"
#include "common.h"

int main()
{
    Client_struct client = {0};
    Server_struct server = {0};

    ctor_client(&client);

    routing_client(&client, &server);

    return 0;
}