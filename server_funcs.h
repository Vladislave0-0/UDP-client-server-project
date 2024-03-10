#ifndef SERVER_H
#define SERVER_H

//===========================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "common.h"

//===========================================================================

int ctor_server(struct Server_struct* server_struct);

void dtor_server(struct Server_struct* server_struct);

int get_ip(struct Server_struct* server_struct);

int Socket(struct Server_struct* server_struct);

int make_fork_proc(struct Server_struct* server_struct, struct list* lst);

int routing_server(struct Server_struct* server_struct, struct list* lst);

int service_server(int fork_val, Status* server_status);

void add_user(struct Server_struct* server_struct, Message* message, struct list* lst);

int check_login(struct Server_struct* server_struct, const char* login, struct list* lst);

void set_online_user_list(struct Server_struct* server_struct, Message* message, struct list* lst);

//===========================================================================

#endif // SERVER_H
