#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#define PORT 2442
#define ADDR "127.0.0.1"
int main()
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    char ch = 'A';
    //create socket
    sockfd =socket(AF_INET,SOCK_STREAM, 0);
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=inet_addr(ADDR);
    address.sin_port=htons(PORT);
    len = sizeof(address);
    
    result=connect(sockfd,(struct sockaddr *)&address, len);
    if(result==-1){
        perror("oops: tcpClientStealed got error in connect function");
        exit(1);
    }
    write(sockfd, &ch,1);
    read(sockfd,&ch,1);
    printf("char from server = %c\n",ch);
    close(0);
}
