#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <netinet/in.h>



int main(int argc , char *argv[])
{

    int port = 8080;

    if (argc == 2)
    {
        port = atoi(argv[1]);
    }

    printf("Lancement du client sur le port %i\n", port);

    //Create socket (slide 16)
    int m_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (m_sock == -1)
    {
        printf("Erreur lors de la création de la socket \n");
        return -1;
    }

    // Create struct sockaddr_in (slide 15)
    printf("Create SIN \n");
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    //sin.sin_addr.s_addr = gethostbyname();
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port);


    // Connect to remote server (slide 23)
    printf("Connection au serveur...\n");
    int isConnect = connect(m_sock, (struct sockaddr *) &sin, sizeof(sin));
    if (isConnect == -1)
    {
        printf("Erreur lors de la connection ! \n");
    }


    // Send some data (slide 26 or 28)
    printf("Préparation send data \n");
    const int tailleBuffer = 1024;
    char *buffer = malloc(tailleBuffer);
    int isLaunch = 1;
    while (isLaunch == 1)
    {
        memset(buffer, 0, sizeof(buffer));
        // buffer = "";
        // buffer = "test";
        printf("Que voulez vous envoyer comme message ? \n");
        fgets(buffer, tailleBuffer, stdin);
        int isWrite = write(m_sock, buffer, strlen(buffer));
        printf("Envoie du buffer effectué : %i \n", isWrite);
    }



    /*
    buffer = "";
    // Receive answer from server (slide 31 or 33)
    ssize_t size_read = read(m_sock, &buffer, tailleBuffer);
    printf("Message reçu de %i bytes : %s \n", size_read, &buffer);
    */

    // Close sockets (slide 17)

    return 0;
}

