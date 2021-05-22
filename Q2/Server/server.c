#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#define Port_No 15050
#define RESET(buff) for(i=0;i<100;i++) buff[i]='\0';

void error(const char *msg){
    perror(msg);
    exit(1);
}

char *crc(char *msg,char *key) {
    char rem[50], temp[100];
    int msg_len, key_len, i;

    msg_len = strlen(msg);
    key_len = strlen(key);
    msg[msg_len] = '\0';
    rem[key_len] = '\0';
    
    // Append 0 at the end
    strcpy(temp, msg);
    for (i = 0; i < key_len-1; i++)
    {
        temp[msg_len + i] = '0';
    }
    
    // XOR operation
    for (int j = 0; j < msg_len; j++)
    {
        if (temp[j] != '0')
        {
            for (int k = 0; k < key_len; k++)
            {
                rem[k] = temp[j+k] = (temp[j+k] == key[k]) ? '0' : '1';
            }
        }
//        printf("%s\n",temp);
//        printf("%s\n",rem);
    }

    // Reduce remainder
    for (int i = 0; i < key_len; i++)
    {
        rem[i] = rem[i+1];
    }
    return strcat(msg,rem);
}

int main(int argc, char const *argv[])
{
    int sockfd,newsockfd,i;
    struct sockaddr_in serv_addr,cli_addr;
    char buff[100],key[100],data[100];
    char *res=malloc(30);
    int clilen;
    
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(Port_No);

    //Socket
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
        error("Error in Socket");

    //Binding
    if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
        error("Error in binding");
    
    //Listen
    listen(sockfd,5);
    
    while(1)
    {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd , (struct sockaddr *)&cli_addr, &clilen);
        if(newsockfd < 0)
            error("Error in Accept");

        RESET(buff)
        strcpy(buff," Enter the data : ");
        send(newsockfd , buff, 100, 0);
        RESET(data)
        recv(newsockfd , data, 100, 0);
        RESET(buff);
        strcpy(buff,"Enter the key : ");
        send(newsockfd, buff, 100, 0);
        RESET(buff);
        recv(newsockfd, buff, 100, 0);
        printf("\n From Client : %s \n",data);
        res = crc(data , buff);
        strcpy(buff,res);
        send(newsockfd , buff, 100 , 0);
        RESET(buff)
        recv(newsockfd , buff, 100, 0);
        printf("\n Message from Client : %s \n",buff);
        close(newsockfd);
    }
    return 0;
}
