#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char *palavra_recebida;

char criptografar_texto(char textoParaCriptografar[10000])
{
    printf("Recebido isso aqui: %s\n", textoParaCriptografar);

    int tamanho_texto = strlen(textoParaCriptografar);
    palavra_recebida[tamanho_texto - 1] = '\0';

    for (int i = 0; i < tamanho_texto; i++)
    {
        char letra = textoParaCriptografar[i];

        letra += 3;

        if (letra > 97)
        {
            letra = letra % 123;

            if (letra > 123)
            {
                letra += 97;
            }
        }
        else
        {
            letra = letra % 91;

            if (letra > 91)
            {
                letra += 65;
            }
        }

        palavra_recebida[i] = letra;
    }

    printf("Final: %s\n", palavra_recebida);
}

int main()
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    char str_in[10000];
    int server_port;

    printf("Digite a porta desse servidor: ");
    scanf("%d", &server_port);

    // Configurando conexão Socket
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(server_port);

    // Bind na conexão
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    listen(server_sockfd, 5);

    while (1)
    {
        printf("Server waiting\n");

        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);

        read(client_sockfd, &str_in, 10000);

        palavra_recebida = malloc(sizeof(char) * strlen(str_in));
        criptografar_texto(str_in);

        write(client_sockfd, palavra_recebida, strlen(palavra_recebida));

        free(palavra_recebida);
        close(client_sockfd);
        memset(str_in, 0, sizeof str_in);
    }
}