#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define IP "172.22.164.52"
#define BUFFER_SIZE 2048

char* get_time(){
        time_t sec;
        time (&sec);
        struct tm * timein;
        timein = localtime(&sec);
        char* now = asctime(timein);
        return now;
}

int main(){
        //create socket
        int state;
        int SFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(SFD == -1){
                printf("socket creation error!\n");
                exit(-1);
        }
        else{
                printf("socket creation successful...\n");
        }

        //bind IP
        struct sockaddr_in Saddr;
        Saddr.sin_family = AF_INET;
        Saddr.sin_port = htons(8080);
        Saddr.sin_addr.s_addr =  htonl(INADDR_ANY);

        state = bind(SFD, (struct sockaddr*)&Saddr, sizeof(struct sockaddr_in));
        if(state == 0){
                printf("IP binding successful...\n");
        }
        else{
                printf("binding error!\n");
                exit(-1);
        }

        //listen
        state = listen(SFD, 10);
        if(state == 0) printf("listening start...\n");
        else{
                printf("listening error!\n");
                exit(-1);
        }
        printf("waiting...\n");

        //until connected then accept
        char buf[BUFFER_SIZE];
        char req_fnc[4];
        while(1){
                        struct sockaddr_in Caddr = {0};
                int Caddrlen = sizeof(Caddr);
                int CFD = accept(SFD, (struct sockaddr*)&Caddr, &Caddrlen);
                if(CFD != -1){
                        printf("accepted\n");
                        //read the request
                        bzero(buf, sizeof(buf));
                        state = read(CFD, buf, BUFFER_SIZE);
                        if(state < 0){
                                printf("reading error!\n");
                        }
                        fprintf(stdout, "%s", buf);
                        strncpy(req_fnc, buf, 4);
                        req_fnc[3] = '\0';
                        if(strcmp(req_fnc, "GET") == 0){
                                char* response = "HTTP/1.1 200 OK\n"
                                        "Date: Sat, 29 Oct 2022 16:50:1 GMT\n"
                                        "Server: Apache/2.2.3\n"
                                        "Last-Modified: Sat, 29 Oct 2022 16:52:1 GMT\n"
                                        "ETag: \"56d-9989a00-1a32c580\"\n"
                                        "Content-Type: text/html\n"
                                        "Content-Length: 256\n"
                                        "Accept-Ranges: bytes\n"
                                        "Connection: close\n"
                                        "\n"
                                        "b08605006\r\n"
                                        "Huang, Shu-Chi\r\n"
                                        "An undergraduate student in NTU\r\n"
                                        "This information is for project of Computer Network\r\n";
                                send(CFD, response, strlen(response), 0);
                                close(CFD);
                        }
                }
        }
        exit(0);

        return 0;
}
