#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>



void port_scanner(char *tip,int first_port, int last_port){

    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    char tport[6] = {0};
    
    int status;
    int sockfd;

    while(first_port <= last_port){
        sprintf( tport, "%d", first_port );
       // strcpy(tport, first_port);
        if ((status = getaddrinfo(tip, tport, &hints, &res)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        }

        for(p = res; p != NULL; p = p->ai_next) {
            if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                perror("server: socket");
                continue;
            }

            status = connect(sockfd, p->ai_addr, p->ai_addrlen);
            if (status<0) 
            {
                printf("Port %s is NOT open.\n", tport);
                close(sockfd);
                continue;
            }

            printf("Port %s is open.\n", tport);
            close(sockfd);
            break;
        }

        if (sockfd < 0) 
        {
            printf("Port %s is NOT open.\n", tport);
            continue; 
        }
        freeaddrinfo(res); // free the linked list
        first_port= first_port + 1;
    }



}




int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf ("Please enter the server IP address"
                " and range of ports to be scanned\n");
        exit(1);
    }
    int startport;
    int endport; 
    startport = atoi(argv[2]);
    endport = atoi(argv[3]);
    char tIP[16] = {0};
    strcpy(tIP, argv[1]); // Copy the IPv4 address
    char First_Port[6] = {0};
    strcpy(First_Port, argv[2]); // Copy the start_port
    char Last_Port[6] = {0};
    strcpy(Last_Port, argv[3]); // Copy the end_port

    // Start port-scanner
    port_scanner(argv[1], startport, endport);
    return 0;
} 
