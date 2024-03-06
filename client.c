#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 51000
#define MAXLINE 1024

int main() {
    int sockfd;
    char message[MAXLINE];
    struct sockaddr_in servaddr = {0};
    
    // Создаём файловый дескриптор UDP-сокета.
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Заполняем структуру для адреса сервера: семейство протоколов, сетевой интерфейс, номер порта.
    servaddr.sin_family = AF_INET;              // IPv4 Internet Protocols
    servaddr.sin_addr.s_addr = INADDR_ANY;      // Не привязываем сокет к определённому IP адресу. При вызове bind() 
                                                // сокет будет принимать соединения со всеми IP-адресами машины.
    servaddr.sin_port = htons(PORT);            // Порты с 49152 по 65535 предназначены для процессов обычных пользователей
    
    while(1)
    {
        printf("Enter message: ");
        fgets(message, MAXLINE, stdin);          // Читаем из терминала сообщение клиента
        if(strcmp(message, "quit\n") == 0)       // Проверяем, не хочет ли клиент выйти с поомщью ключевого слова "quit"
        {
            close(sockfd);                       // Закрываем дискриптор сокета по окончании работы 
            return 0;
        }
        // Отправляем датаграмму на обработку серверу из буффера message
        sendto(sockfd, (const char *)message, strlen(message), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    }
    // Закрываем дискриптор сокета по окончании работы 
    close(sockfd);  
    return 0;
}
