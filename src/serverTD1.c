#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <stdlib.h>
#include <pthread.h>


int main(int argc , char *argv[])
{

    int port = 8080;
    int isRun = 1;

    if (argc == 2)
    {
        port = atoi(argv[1]);
    }

    printf("Lancement du serveur sur le port %i\n", port);

    // Create socket (slide 16)
    int m_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (m_sock == -1)
    {
        printf("Erreur lors de la création de la socket \n");
        return -1;
    }
    // htonl(INADDR_ANY);

    // Prepare the sockaddr_in structure (slide 15)
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port);


    // Bind (slide 20)
    int is_bind = bind(m_sock, (struct sockaddr *) &sin, sizeof(sin));
    if (is_bind == -1)
    {
        printf("Erreur lors du bind \n");
        return -1;
    }


    // Listen (slide 21)
    printf("Ecoute...\n");
    int is_listen = listen(m_sock, 10);
    if (is_listen == -1)
    {
        printf("Erreur lors du listen \n");
        return -1;
    }

    printf("Création socket client\n");
    // Accept incoming connection (slide 22)
    struct sockaddr_in csin = { 0 };
    size_t sinsize = sizeof(csin);
    printf("Accept\n");
    const int tailleBuffer = 1024;
    char *buffer = malloc(tailleBuffer);

    while (isRun == 1)
    {
        memset(buffer, 0, sizeof(buffer));
        int sock_client = accept(m_sock, (struct sockaddr *) &csin, &sinsize);
        if (sock_client == -1)
        {
            printf("Erreur accept client \n");
            continue;
        }


        // Receive message from client (slide 31 or 33)
        ssize_t size_read = read(sock_client, buffer, tailleBuffer);
        printf("Nombre de byte lu : %i \n", size_read);
        printf("Message reçu : %s !\n", buffer);

        if (strcmp(buffer, "/quit\n") == 0)
        {
            printf("Server stopping...\n");
            isRun = -1;
        }

        // Send answer to client (slide 26 or 328)
        printf("Envoie du message %s sur le client \n", buffer);
        ssize_t size_write = write(sock_client, buffer, strlen(buffer));
        printf("Nombre de byte envoyé : %i \n", size_write);

        close(sock_client);

    }

    /**
        Close server
    **/

    close(m_sock);
    buffer = NULL;
    free(buffer);
    return 0;
}
