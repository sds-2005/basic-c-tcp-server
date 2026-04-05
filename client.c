#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>


void error(const char* msg)
{
    perror(msg);
    exit(1);
}

// 3 inputs   1. file name   2. server ipaddres   3.port no 

int main(int argc,char* argv[])
{
    if (argc<3)
    {
        fprintf(stderr,"usage %s hostname port\n",argv[0]);
        exit(1);
    }
    int sockfd,portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;//stores info about the host in this case the server
    //definition 
    /*
    struct hostent {
    char  *h_name;        // official host name
    char **h_aliases;     // alias list
    int    h_addrtype;    // address type (AF_INET)
    int    h_length;      // length of address
    char **h_addr_list;   // list of IP addresses
    };

    */
    char buffer[255];

    portno=atoi(argv[2]);

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if (sockfd<0)
    {
        error("Error opening Socket\n");
    }

    //getting server info
    server=gethostbyname(argv[1]);
    if (server==NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        exit(1);
    }

    bzero((char*) &serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;

    //copying server's ip address to serv_addr from server
    bcopy((char*) server->h_addr,(char*) &serv_addr.sin_addr.s_addr,server->h_length);

    serv_addr.sin_port=htons(portno);



    if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
    error("Connection Failed\n");

    int n;

    while (1)
    {
        bzero(buffer,255);
        fgets(buffer,255,stdin);
        n=write(sockfd,buffer,strlen(buffer));
        if (n<0)
        error("Write Failed\n");

        bzero(buffer,255);
        n=read(sockfd,buffer,255);
        if (n<0)
        error("Read Failed\n");

        printf("Server : %s\n",buffer);

        int i=strncmp("Bye",buffer,3);
        if (i==0)
        break;

    }

    close(sockfd);

    return 0;





}
