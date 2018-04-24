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
    const int tailleBody = 1024;
    const int tailleHeader = 1024;
    const int tailleResponse = 2048;
    char *header = malloc(tailleHeader);
    char *body = malloc(tailleBody);
    char *response = malloc(tailleResponse);

    while (isRun == 1)
    {
        printf("Memset header\n");
        //memset(header, 0, sizeof(header));
        memset(header, '\0', sizeof(header));
        printf("Accept...");
        int sock_client = accept(m_sock, (struct sockaddr *) &csin, &sinsize);
        printf("Done ! \n");
        if (sock_client == -1)
        {
            printf("Erreur accept client \n");
            continue;
        }



        // strcpy(body, "test");
        body = "<h1>test</h1><p>Je suis une page genere en <strong>C</strong></p><a href=\"http://google.fr\">Google</a>";

        strcpy(header, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nAccept-Charset: utf-8\r\nContent-Length: \0");
        char content_size[10];
        sprintf(content_size, "%d", strlen(body));

        printf("Content_size = %s \n", content_size);

        strcat(header, content_size);

        printf("HEADER = %s\n", header);

        strcat(header, "\r\n\r\n");
        strcpy(response, "");
        // response = "";
        strcat(response, header);
        strcat(response, body);

        printf("Envoie réponse\n");
        printf("Header : %s \n", response);

        ssize_t size_write = write(sock_client, response, strlen(response));
        printf("Nombre de byte envoyé : %i \n", size_write);

        close(sock_client);

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
    header = NULL;
    free(header);
    return 0;
}
