#include "server_funcs.h"

//============================================================================

int ctor_server(struct Server_struct* server_struct)
{
    #ifdef LOOP_BACK
        stpcpy(server_struct->ip_str, "INADDR_LOOPBACK");
    #else
        get_ip(server_struct);
    #endif

    server_struct->port = DEFAULT_SERVER_PORT;

    printf("Server IP: %s\n", server_struct->ip_str);
    printf("Server port: %lu\n", server_struct->port);


    server_struct->cliarr = (struct Client_struct*)calloc(MAX_USERS_NUMBER, sizeof(struct Client_struct));

    if(server_struct->cliarr == nullptr)
    {
        printf("ERROR CLIARR CALLOC!\n");
        return ERROR_CLIARRAY_CALLOC;
    }

    return SUCCESS;
}

//============================================================================

void dtor_server(struct Server_struct* server_struct)
{
    free(server_struct->cliarr);
    server_struct->cliarr = nullptr;
    server_struct->cur_users = 0;
    strcpy(server_struct->ip_str, "");
    close(server_struct->socketfd);
    server_struct->socketfd = 0;
}

//============================================================================

int get_ip(struct Server_struct* server_struct)
{
    char hostname[256];
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    int status;

    if (gethostname(hostname, sizeof(hostname)) == -1) {
        perror("Error getting hostname");
        return 1;
    }

    // Заполнение структуры hints для getaddrinfo()
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Получает информацию об адресе с учетом информации в hints. 
    // Результат сохраняется в структуре res. Если есть ошибка при 
    // получении информации об адресе, программа выводит сообщение 
    // об ошибке и завершает выполнение.
    if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    struct sockaddr_in* ipv4 = (struct sockaddr_in*)res->ai_addr;
    void* addr = &(ipv4->sin_addr);

    // Преобразование IP-адреса в удобочитаемый формат
    inet_ntop(res->ai_family, addr, server_struct->ip_str, sizeof(server_struct->ip_str));
    // printf("From get_ip: %s\n", server_struct->ip_str);

    freeaddrinfo(res); // Очистка структуры res

    return 0;
}

//============================================================================

int Socket(struct Server_struct* server_struct)
{
    server_struct->servaddr.sin_port = htons(server_struct->port);
    server_struct->servaddr.sin_family = AF_INET;
    server_struct->socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(server_struct->socketfd == -1)
    {
        printf("ERROR SOCKET CREATION!\n");
        return ERROR_SOCKET_CREATION;
    }

    #ifdef LOOP_BACK
    server_struct->servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    #else
    if(inet_aton(server_struct->ip_str, &server_struct->servaddr.sin_addr) == 0)
    {
        printf("ERROR CONVERTATION INTERNET HOST ADDRESS!\n");
        return ERROR_INET_ATON;
    }
    #endif

    if(bind(server_struct->socketfd, (struct sockaddr*)&server_struct->servaddr, sizeof(server_struct->servaddr)) == -1)
    {
        printf("ERROR BIND SOCKET! SOCKET IS ALREADY IN USE!\n");
        return ERROR_SOCKET_BIND;
    }

    return SUCCESS;
}

//============================================================================

int make_fork_proc(struct Server_struct* server_struct)
{
    int server_pid = getpid();
    int ret = fork();

    if(ret == -1)
    {
        printf("ERROR FORK PROCESSES!\n");
        return ERROR_FORK_PROC;
    }

    // else
    Status server_status = ONLINE;
    int pid  = getpid();
    int ppid = getppid();

    while(server_status)
    {
        if(pid == server_pid)       // process is parent
        {
            int error_code = service_server(ret, &server_status);
            return error_code;
        }

        else if(ppid == server_pid) // process is child
        {
            routing_server(server_struct);
        }
    }

    return SUCCESS;
}

//============================================================================

int service_server(int fork_val, Status* server_status)
{
    char command[MAX_COMMAND_LENGTH] = {0};

    while(*server_status)
    {
        printf("Enter command: ");
        scanf("%s", command);

        if(strcmp(command, "kill") == 0)
        {
            *server_status = OFFLINE;
            if(kill(fork_val, SIGTERM) == -1)
            {
                printf("ERROR FORK KILL!\n");
                return ERROR_KILL_FORK;
            }
            break;
        }
    }

    return SUCCESS;
}

//============================================================================

int routing_server(struct Server_struct* server_struct)
{
    Message message;
    struct sockaddr_in cliaddr = {0};
    socklen_t sock_len = sizeof(cliaddr);

    while(true)
    {
        int recv_ret = recvfrom(server_struct->socketfd, (Message*)&message, sizeof(message), 0, (struct sockaddr*)(&cliaddr), &sock_len);

        if (recv_ret != -1)
        {
            switch(message.req)
            {
                case(REGISTRATION_REQUEST):
                {
                    add_user(server_struct, &message);
                    break;
                }
            
                default:
                {
                    break;
                }
            }
        }

        else
        {
            printf("In routing_server: recvfrom error!\n");
        }
    }

    return SUCCESS;
}

//============================================================================

void add_user(struct Server_struct* server_struct, Message* message)
{
    Client_struct client_struct;
    Message new_msg;
    client_struct.cliaddr.sin_port   = htons(message->sender.port);
    client_struct.cliaddr.sin_family = AF_INET;

    #ifdef LOOP_BACK
    client_struct.cliaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    #else
    if(inet_aton(client_struct.ip_str, &client_struct.cliaddr.sin_addr) == 0)
    {
        printf("ERROR CONVERTATION INTERNET HOST ADDRESS!\n");
    }
    #endif


    if(server_struct->cur_users >= MAX_USERS_NUMBER)
    {
        new_msg.ans = SERVER_IS_FULL;
        int send_ret = sendto(server_struct->socketfd, (Message*)&new_msg, sizeof(new_msg), 0, (const sockaddr*)(&client_struct.cliaddr), sizeof(client_struct.cliaddr));
        return;
    }

    if(check_login(server_struct, message->sender.login) != SUCCESS)
    {
        new_msg.ans = LOGIN_ALREADY_USED;
        int send_ret = sendto(server_struct->socketfd, (Message*)&new_msg, sizeof(new_msg), 0, (const sockaddr*)(&client_struct.cliaddr), sizeof(client_struct.cliaddr));
        return;
    }

    
    Client_struct* new_client = &server_struct->cliarr[server_struct->cur_users];
    new_client->port = message->sender.port;
    strcpy(new_client->login, message->sender.login);
    strcpy(new_client->ip_str, message->sender.ip_str);
    new_client->socketfd = message->sender.socketfd;
    new_client->cliaddr.sin_addr = message->sender.cliaddr.sin_addr;
    new_client->cliaddr.sin_family = message->sender.cliaddr.sin_family;
    new_client->cliaddr.sin_port = message->sender.cliaddr.sin_port;
    new_msg.ans = APPROVED;
    server_struct->cur_users++;

    printf("\n======New user entered=====\n");
    printf("Login: %s\n", new_client->login);
    printf("IP: %s\n", new_client->ip_str);
    printf("Port: %lu\n", new_client->port);
    printf("\n===========================\n");


    int send_ret = sendto(server_struct->socketfd, (Message*)&new_msg, sizeof(new_msg), 0, (const sockaddr*)(&client_struct.cliaddr), sizeof(client_struct.cliaddr));
}

//============================================================================

int check_login(struct Server_struct* server_struct, const char* login)
{
    int login_used = 0;

    for(size_t i = 0; i < MAX_USERS_NUMBER; i++)
    {
        if(strcmp(server_struct->cliarr[i].login, login) == 0)
        {
            login_used = 1;
            return -1;
        }
    }

    return SUCCESS;
}

//============================================================================

