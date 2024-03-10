#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include "common.h"

//===========================================================================

int ctor_client(struct Client_struct* client_struct);

int get_ip(struct Client_struct* client_struct);

void change_port(struct Client_struct* client_struct);

int routing_client(struct Client_struct* client_struct, struct Server_struct* server_struct);

void user_help();

int Socket(struct Client_struct* client_struct);

int server_registration(struct Client_struct* client_struct, struct Server_struct* server_struct);

//===========================================================================

#endif //CLIENT_H
