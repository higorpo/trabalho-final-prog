
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int servers[] = {9734, 9735};
    int qtd_servers = sizeof(servers) / sizeof(servers[0]);

    struct timeval t1, t2;

    int sockfd[qtd_servers];
    int len[qtd_servers];
    struct sockaddr_in address[qtd_servers];
    int result[qtd_servers];

    char palavra_sem_criptografia[10000];
    char palavra_criptografada[10000];

    // Lendo dados para enviar
    printf("Digite a palavra a ser criptografada: ");
    scanf("%s", palavra_sem_criptografia);

    // Divide a palavra em segmentos de acordo com a quantidade de servidores disponíveis
    int max_bytes = strlen(palavra_sem_criptografia) / qtd_servers;

    char segmentos[qtd_servers][max_bytes];

    for (int i = 0; i < qtd_servers; i++)
    {
        char substring[max_bytes];
        strncpy(substring, &palavra_sem_criptografia[i * max_bytes], max_bytes);
        substring[max_bytes] = '\0';

        strncpy(segmentos[i], substring, max_bytes);
        printf("%d - Recebido da string %s\n\n\n", i, segmentos[i]);
        memset(substring, 0, sizeof substring);
    }

    // Configurando socket
    for (int i = 0; i < qtd_servers; i++)
    {
        printf("Conectando-se via socket com o servidor %d - %d\n", servers[i], i);
        sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
        address[i].sin_family = AF_INET;
        address[i].sin_addr.s_addr = inet_addr("127.0.0.1");
        address[i].sin_port = htons(servers[i]);

        len[i] = sizeof(address);
        result[i] = connect(sockfd[i], (struct sockaddr *)&address[i], len[i]);

        if (result[i] == -1)
        {
            perror("oops: não foi possível se conectar via socket\n");
            exit(1);
        }
    }

    gettimeofday(&t1, NULL);

    // Enviando dados
    for (int i = 0; i < qtd_servers; i++)
    {
        printf("Enviando dados para o servidor %d - %d | Dados: %s\n", servers[i], i, segmentos[i]);
        write(sockfd[i], segmentos[i], max_bytes);
    }

    // Recebendo dados
    char str_out[10000];
    for (int i = 0; i < qtd_servers; i++)
    {
        printf("Recebendo dados do servidor %d - %d\n", servers[i], i);
        read(sockfd[i], &str_out, 10000);

        strncat(palavra_criptografada, str_out, strlen(str_out));
        close(sockfd[i]);
        memset(str_out, 0, sizeof str_out);
    }

    gettimeofday(&t2, NULL);

    double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);

    printf("Mensagem criptografada: %s\n", palavra_criptografada);
    printf("Tempo para execução: %lf+\n", t_total);
    exit(0);
}