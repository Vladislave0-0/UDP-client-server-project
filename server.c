#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 51000
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr = {0};
    
    // Создаём файловый дескриптор UDP-сокета.
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Заполняем структуру для адреса сервера: семейство протоколов, сетевой интерфейс, номер порта.
    servaddr.sin_family = AF_INET;              // IPv4 Internet Protocols
    servaddr.sin_addr.s_addr = INADDR_ANY;      // Не привязываем сокет к определённому IP адресу. При вызове bind() 
                                                // сокет будет принимать соединения со всеми IP-адресами машины.
    servaddr.sin_port = htons(PORT);            // Порты с 49152 по 65535 предназначены для процессов обычных пользователей

    // Настройка адреса сокета с помощью bind(). Привязка сокета к IP адресу и номеру порта.
    if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
    {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    int len = sizeof(cliaddr);
    int n_bytes = 0;
    
    while(1) 
    {
        // Чтение принятой датаграммы
        n_bytes = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n_bytes] = '\0';
        printf("Client : %s\n", buffer);
    }
    
    return 0;
}