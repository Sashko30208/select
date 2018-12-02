#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#define PORT 2442
//int main(int argc, char const *argv[])
int main()
{
    int server_sockfd,client_sockfd;
    int server_len,client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int result;
    fd_set readfds, testfds;
    
    /*creating a socket for server and naming it*/
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);//sock_stream->tcp

    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);
    server_len = sizeof(server_address);
    
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

    //making a queue of query for connect
    listen(server_sockfd,5);

    //нахрен инглиш
    //инициализирую "readfds" для обработки ввода с сокета "server_sockfd"
    FD_ZERO(&readfds);
    FD_SET(server_sockfd, &readfds);

    while(1){
        char ch;
        int fd;
        int nread;

        testfds = readfds;

        printf("server waiting\n");
        result = select(FD_SETSIZE,&testfds,(fd_set *)0,(fd_set *)0, (struct timeval *) 0);
        if(result<1){
            perror("select on tspselserv<1");
            exit(1);
        }

        //проверяем какой из дискриптеров активен
        for(fd=0;fd<FD_SETSIZE;fd++){
            if(FD_ISSET(fd,&testfds)){
                /*если активность на "server_sockfd",
                 то это может быть запрос на новое соединение */
                if(fd==server_sockfd){
                    client_len=sizeof(client_address);
                    client_sockfd=accept(server_sockfd, (struct sockaddr *)&client_address,&client_len);
                    FD_SET(client_sockfd, &readfds);
                    printf("adding client on fd %d\n", client_sockfd);
                }
                /*
                если активен не сервер, то это клиент
                */
               else {
                   ioctl(fd,FIONREAD, &nread);
                   /*
            in <sys/ioctl.h>
            #define	FIONREAD	_IOR('f', 127, int)	/ get # bytes to read 
            */
                   if(nread == 0){
                       close(fd);
                       FD_CLR(fd,&readfds);
                       printf("removing client on %d\n",fd);
                   }
                   else{
                       read(fd,&ch,1);
                       sleep(5);
                       printf("serving client on fd %d\n",fd);
                       ch++;
                       write(fd,&ch,1);
                   }

               }
            }
        }
    }
}
