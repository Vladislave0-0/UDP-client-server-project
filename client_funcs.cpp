#include "client_funcs.h"

//============================================================================

int ctor_client(struct Client_struct* client_struct)
{
    #ifdef LOOP_BACK
        stpcpy(client_struct->ip_str, "INADDR_LOOPBACK");
    #else
        get_ip(client_struct);
    #endif

    client_struct->port = DEFAULT_USERS_PORT;

    printf("Your IP: %s\n", client_struct->ip_str);
    printf("Your port: %lu\n", client_struct->port);

    change_port(client_struct);

    printf("\nWrite your username (maximum length of the login is 16 characters): ");
    scanf("%s", client_struct->login);
    client_struct->login[MAX_LOGIN_LENGTH - 1] = '\0';

    return SUCCESS;
}

//============================================================================

int get_ip(struct Client_struct* client_struct)
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
    inet_ntop(res->ai_family, addr, client_struct->ip_str, sizeof(client_struct->ip_str));
    // printf("From get_ip: %s\n", server_struct->ip_str);

    freeaddrinfo(res); // Очистка структуры res

    return 0;
}

//============================================================================

void change_port(struct Client_struct* client_struct)
{
    char command;
    printf("\nDo you want to change the value of your port? (y/n): ");
    scanf("%c", &command);

    switch(command)
    {
        case('y'):
        {
            int new_port = 0;
            printf("\nEnter new port value: ");
            scanf("%d", &new_port);

            if(new_port > 0)
            {   
                client_struct->port = new_port;
                printf("\nNow you have port %lu!\n", client_struct->port);
            }
            else
            {        
                printf("\nInvalid port value. Your port remains the same!\n");
            }

            break;
        }
        case('n'):
        {
            printf("\nYour port remains the same!\n");
            break;
        }
        default:
        {
            printf("\nInvalid command. Your port remains the same!\n");
            break;
        }
    }
}

//============================================================================

int routing_client(struct Client_struct* client_struct, struct Server_struct* server_struct)
{
    user_help();
    int is_registered  = 0;
    int socket_created = 0;

    char command[MAX_COMMAND_LENGTH];

    while(1)
    {
        printf("Enter command: ");
        scanf("%s", command);

        switch(command[0])
        {
            case('k'):
            {
                return SUCCESS;
            }

            case('m'):
            {
                break;
            }

            case('r'):
            {
                if(is_registered == 0)
                {
                    if(socket_created == 0 && Socket(client_struct) != SUCCESS)
                    {
                        printf("\nERROR IN ROUTING CLIENT!\n\n");
                        return -1;
                    }
                    socket_created = 1;
                    if(server_registration(client_struct, server_struct) == SUCCESS)
                    {
                        is_registered = 1;
                    }
                }
                else
                {
                    printf("\nYou are already registered!\n");
                }
                break;
            }

            case('h'):
            {
                user_help();
                break;
            }

            case('l'):
            {
                if(is_registered == 1)
                {
                    req_user_list(client_struct, server_struct);
                }
                else
                {
                    printf("\nConnect to the server to see the list of active users!\n");
                }
                break;
            }
            
            default:
            {
                printf("\nUNKNOWN COMMAND. TRY AGAIN!\n");
                break;
            }
        }
    }

    return SUCCESS;
}

//============================================================================

void user_help()
{
    printf("\nIT IS HELP INSTRUCTION!\n");
    printf("h - print help one more time\n");
    printf("r - server registration\n");
    printf("l - list with online users\n");
    printf("m - send a message\n");
    printf("k - kill the programm\n\n");
}

//============================================================================

int Socket(struct Client_struct* client_struct)
{
    client_struct->cliaddr.sin_family = AF_INET;
    client_struct->cliaddr.sin_port   = htons(client_struct->port);
    client_struct->socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(client_struct->socketfd == -1)
    {
        printf("ERROR SOCKET CREATION!\n");
        return ERROR_SOCKET_CREATION;
    }

    #ifdef LOOP_BACK
    client_struct->cliaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    #else
    if(inet_aton(client_struct->ip_str, &client_struct->cliaddr.sin_addr) == 0)
    {
        printf("ERROR CONVERTATION INTERNET HOST ADDRESS!\n");
        return ERROR_INET_ATON;
    }
    #endif

    if(bind(client_struct->socketfd, (struct sockaddr*)&client_struct->cliaddr, sizeof(client_struct->cliaddr)) == -1)
    {
        printf("ERROR BIND SOCKET! SOCKET IS ALREADY IN USE!\n");
        return ERROR_SOCKET_BIND;
    }

    return SUCCESS;
}

//============================================================================

int server_registration(struct Client_struct* client_struct, struct Server_struct* server_struct)
{
    // server settings
    strcpy(server_struct->ip_str, client_struct->ip_str);
    server_struct->servaddr.sin_port   = htons(DEFAULT_SERVER_PORT);
    server_struct->servaddr.sin_family = AF_INET;

    #ifdef LOOP_BACK
        server_struct->servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    #else

        inet_aton(server_struct->ip_str, &server_struct->servaddr.sin_addr);
    #endif

    // Registration message settings
    Message message;
    message.req = REGISTRATION_REQUEST;
    strcpy(message.sender.login, client_struct->login);
    strcpy(message.sender.ip_str, client_struct->ip_str);
    message.sender.port = client_struct->port;
    message.sender.cliaddr.sin_port = client_struct->cliaddr.sin_port;
    message.sender.socketfd = client_struct->socketfd;

    int send_ret = sendto(client_struct->socketfd, (Message*)&message, sizeof(message), 0, (const struct sockaddr*)(&server_struct->servaddr), sizeof(server_struct->servaddr));

    if(send_ret == -1)
    {
        printf("\nIn register_on_server: sendto error!\n");
        return -1;
    }

    struct sockaddr temp_struct;
    socklen_t sock_len = sizeof(temp_struct);

    int recv_ret = recvfrom(client_struct->socketfd, (Message*)&message, sizeof(message), 0, &temp_struct, &sock_len);
    if (recv_ret != -1)
    {        
        switch(message.ans)
        {
            case(APPROVED):
            {
                printf("\nYou have been successfully registered on the server!\n");
                // server_struct->cur_users++;
                return SUCCESS;
            }
            case(SERVER_IS_FULL):
            {
                printf("\nThere are no places on the server at the moment. Wait a bit.\n");
                return -1;
            }
            case(LOGIN_ALREADY_USED):
            {   
                printf("\nThere is a user with the login %s on the server!\nPlease change your user name: ", client_struct->login);
                scanf("%s", client_struct->login);
                client_struct->login[MAX_LOGIN_LENGTH - 1] = '\0';
                return -1;
            }
        }
    }
    else
    {
        printf("\nERROR MESSAGE\n");
        return -1;
    }

    return SUCCESS;
}

//============================================================================

void req_user_list(struct Client_struct* client_struct, struct Server_struct* server_struct)
{
    // server settings
    strcpy(server_struct->ip_str, client_struct->ip_str);
    server_struct->servaddr.sin_port   = htons(DEFAULT_SERVER_PORT);
    server_struct->servaddr.sin_family = AF_INET;

    #ifdef LOOP_BACK
        server_struct->servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    #else

        inet_aton(server_struct->ip_str, &server_struct->servaddr.sin_addr);
    #endif

    // Registration message settings
    Message message;
    message.req = ONLINE_USER_LIST_REQUEST;
    strcpy(message.sender.login, client_struct->login);
    strcpy(message.sender.ip_str, client_struct->ip_str);
    message.sender.port = client_struct->port;
    message.sender.cliaddr.sin_port = client_struct->cliaddr.sin_port;
    message.sender.socketfd = client_struct->socketfd;

    int send_ret = sendto(client_struct->socketfd, (Message*)&message, sizeof(message), 0, (const struct sockaddr*)(&server_struct->servaddr), sizeof(server_struct->servaddr));

    if(send_ret == -1)
    {
        printf("\nIn req_user_list: sendto error!\n");
        return;
    }

    struct sockaddr temp_struct;
    socklen_t sock_len = sizeof(temp_struct);
    char buffer[MAX_USERS_NUMBER][MAX_LOGIN_LENGTH] = {0};


    int recv_ret = recvfrom(client_struct->socketfd, (char**)buffer, sizeof(buffer), 0, &temp_struct, &sock_len);
    if (recv_ret != -1)
    {
        printf("\n=====Online users=====\n");
        for(size_t i = 0; i < buffer[i][0] != '\0' && i < MAX_USERS_NUMBER; i++)
        {
            if(strcmp(buffer[i], client_struct->login) == 0)
            {
                printf("%s (you)\n", buffer[i]);
            }
            else
            {
                printf("%s\n", buffer[i]);
            }
        }
        printf("======================\n");
    }
    else
    {
        printf("\nIn req_user_list: ERROR MESSAGE\n");
    }

}

//============================================================================
