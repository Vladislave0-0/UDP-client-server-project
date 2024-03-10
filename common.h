#ifndef COMMON_H
#define COMMON_H

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


// #define DEFAULT_SERVER_IP   "10.55.135.60"
#define DEFAULT_SERVER_PORT 51000  
#define DEFAULT_USERS_PORT  52000
#define MAX_LOGIN_LENGTH    24      
#define MAX_USERS_NUMBER    10
#define MAX_COMMAND_LENGTH  8

#define LOOP_BACK



enum Errors
{
    SUCCESS,
    ERROR_CLIARRAY_CALLOC,
    ERROR_SOCKET_CREATION,
    ERROR_INET_ATON,
    ERROR_SOCKET_BIND,
    ERROR_FORK_PROC,
    ERROR_KILL_FORK,
};

enum Status
{
    OFFLINE = 0,
    ONLINE  = 1,
};

// enum MessageType
// {
//     UNKNOWN      = 0,
//     REGISTRATION = 1,
// };

enum Request
{
    // DENIED   = 0,
    // APPROVED = 1,

    REGISTRATION_REQUEST = 2,
    
};

enum Answer
{
    DENIED   = 0,
    APPROVED = 1,

    SERVER_IS_FULL     = 2,
    LOGIN_ALREADY_USED = 3,
};

struct Client_struct
{
    struct sockaddr_in cliaddr   = {0};
    char ip_str[INET_ADDRSTRLEN] = {0};
    char login[MAX_LOGIN_LENGTH] = {0};

    int socketfd = 0;
    size_t port  = 0;
};

struct Message
{
    struct Client_struct sender   = {0};
    struct Client_struct receiver = {0};
    // MessageType type = UNKNOWN;
    Request req;
    Answer  ans;
};

struct Server_struct
{
    struct sockaddr_in servaddr  = {0};
    struct Client_struct* cliarr = {0};
    char ip_str[INET_ADDRSTRLEN] = {0};

    size_t cur_users = 0;
    int socketfd = 0;
    size_t port  = 0;
};



#endif // COMMON_H
