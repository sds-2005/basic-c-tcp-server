#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
/*
    steps
    1. create socket 
    2. bind the socket to address 
    3. listen for connections
    4. accept the connection
    5. read 
    6. write
    7. goto5 until comm finishes i.e termination

*/


void error (const char *msg)
{
    perror(msg);//gives the error message according to errno generated (happens in backend)
    exit(1);
}

//command line arguments 
// we take 2 arguments that are   1. file name      and      2. port no
int main(int argc,char* argv[])
{
    if (argc<2)
    {
        //fprintf writes to a file, stderr is os stream of data type File* i.e pointer to a file
        fprintf(stderr,"Port Number not provided\n");
        exit(1);
    }

    int sockfd,newsockfd,portno;
    char buffer[255];// for communication

    // struct sockaddr_in (used for IPv4  , its sockaddr_in6 for IPv6) is a structure describing an internet socked address
    /*
       //format
        struct sockaddr_in
        {
            sa_family_t sin_family; // address family (AF_INET) for ipv4
            in_port_t sin_port; // port number
            struct in_addr sin_addr; //ip address
            unsigned char sin_zero[8];
        }
    */
    
    
    
    struct sockaddr_in serv_addr,cli_addr;
    socklen_t clilen;

    /// type=Sock_stream , protocol =0  for tcp
    // sockfd is just an integer representing socket file descriptor
    sockfd=socket(AF_INET,SOCK_STREAM,0);

    if (sockfd<0)
    {
        error("Error opening socket\n");
    }

    // Since char is of 1 byte bzero clears the memory byte by byte 
    bzero ((char*) &serv_addr,sizeof(serv_addr));
    portno=atoi(argv[1]);

    serv_addr.sin_family=AF_INET;
    // This specifies the machine to accept connections to local ip of this machine
    serv_addr.sin_addr.s_addr= INADDR_ANY;
    // htons (host to network short) used for converting given host formattiong to BIG ENDIAN (Network Byte Order)formatting that is transmitted over the physical layer 
    serv_addr.sin_port=htons(portno);
    // Vice versa ntohs (network to host short) is used for converting BIG ENDIAN to host formatting ussually done when data is recieved from physical layer
    

    if (bind(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr))<0)
    {
        error("Binding Failed\n");
    }

    listen(sockfd,5);//fd, backlog (max connections to device at a time)

    clilen=sizeof(cli_addr);
    
    newsockfd=accept(sockfd,(struct sockaddr*)& cli_addr,&clilen);
    if (newsockfd<0)
    {
        error("Acceptance Failed\n");
    }

    int n;

    while (1)
    {
        // clear the buffer and read
        bzero(buffer,255);
        n=read(newsockfd,buffer,255);
        if (n<0)
        {
            error("Reading failed\n");
        }
        printf("Client : %s\n",buffer);
        
        // clear the buffer and write to it
        bzero(buffer,255);
        fgets(buffer,255,stdin);
        n=write(newsockfd,buffer,strlen(buffer));
        if (n<0)
        {
            error("Writing Failed");
        }
        
        // termination condition bye
        int i=strncmp("Bye",buffer,3);
        if (i==0)
        break;
    }

    close(sockfd);
    close(newsockfd);
    return 0;

}