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
    fd_set fd_write;
    fd_set fd_read;
    int m_clients[256];
    int m_sockets[257];
    int nb_clients = 0;

    FD_ZERO(&fd_write);
    FD_ZERO(&fd_read);

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
    FD_SET(m_sock, &fd_read);
    m_sockets[0] = m_sock;
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
        int i = 0;
        for (i = 0; i < nb_clients + 1; i++)
        {
            FD_SET(m_sockets[i], &fd_read);
        }
        printf("Attente du SELECT\n");
        if (select(FD_SETSIZE, &fd_read, NULL, NULL, NULL) < 0)
        {
            perror("Erreur dans le select\n");
            return -1;
        }
        // Vérifier si c'est un event du socket serveur (connection d'un client)
        if (FD_ISSET(m_sock, &fd_read))
        {
            printf("CONNEXION D'UN CLIENT\n");
            m_clients[nb_clients] = accept(m_sock, (struct sockaddr *) &csin, &sinsize);
            FD_SET(m_clients[nb_clients], &fd_read);
            m_sockets[nb_clients + 1] = m_clients[nb_clients];
            nb_clients++;
            // close(sock_client);
        }
        else
        {
            printf("ELSE\n");
            int i = 0;
            for (i = 0; i < nb_clients; i++)
            {
               if (FD_ISSET(m_clients[i], &fd_read))
               {
                    printf("Message du client numéro : %i \n", i);
                    ssize_t size_read = read(m_clients[i], buffer, tailleBuffer);
                    if (size_read == 0)
                    {
                        printf("Delete client\n");
                        FD_CLR(m_clients[i], &fd_read);
                        close(m_clients[i]);
                        // m_sockets[i + 1] = -1;
                        nb_clients--;
                        break;
                    }
                    printf("Nombre de byte lu : %i \n", size_read);
                    printf("Message reçu : %s !\n", buffer);
                    break;
               }
            }
        }




        // int sock_client = accept(m_sock, (struct sockaddr *) &csin, &sinsize);


        // close(sock_client);

        /*
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
        */

    }

    /**
        Close server
    **/

    close(m_sock);
    buffer = NULL;
    free(buffer);
    return 0;
}
