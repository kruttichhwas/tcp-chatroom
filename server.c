#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Port Number not provided. Program terminated\n");
        exit(1);
    }

    int sockfd, newsockfd, portno, n;
    char buffer[255];
    char name[255];
    char client_name[255];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Some error occured.");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Error while binding.");
    }

    listen(sockfd, 5);

    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0)
    {
        error("Error while accepting connection");
    }
    printf("Enter your name: ");
    fgets(name, 255, stdin);
    name[strcspn(name, "\n")] = 0;
    n = read(newsockfd, client_name, 255);
    if (n < 0)
    {
        error("Error while reading.");
    }
    n = write(newsockfd, name, 255);
    if (n < 0)
    {
        error("Error while writing.");
    }

    printf("-----------------------------\n");
    while (1)
    {
        bzero(buffer, 255);
        n = read(newsockfd, buffer, 255);
        if (n < 0)
        {
            error("Error while reading.");
        }
        printf("%s: %s", client_name, buffer);
        bzero(buffer, 255);
        printf("You: ");
        fgets(buffer, 255, stdin);
        n = write(newsockfd, buffer, 255);
        if (n < 0)
        {
            error("Error while writing.");
        }
        int i = strncmp("Bye", buffer, 3);
        if (i == 0)
        {
            break;
        }
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}
