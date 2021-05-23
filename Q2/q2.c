// For both
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// For server
#include <netdb.h>

// For client
#define PORT 5000
#define BUF_SIZE 256

#define RESET(buff) for(int i=0;i<100;i++) buff[i]='\0';

void rand_string(char *str, size_t size)
{
    const char charset[] = "01";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
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
    }

    // Reduce remainder
    for (int i = 0; i < key_len; i++)
    {
        rem[i] = rem[i+1];
    }
    return strcat(msg,rem);
}

int client()
{
    /* Create a socket first */
    int sockfd = 0;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    /* Initialize sockaddr_in data structure */
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT); // port
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    /* Attempt a connection */
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }
    
    char buff[BUF_SIZE];
    memset(buff, '0', sizeof(buff));
    
    // Divisor for the string
    char div[BUF_SIZE];
    printf("Enter any divisor of length 5\n");
    scanf("%s",&div);
    
    write(sockfd, div, sizeof(buff));
    RESET(buff);
    read(sockfd, buff, sizeof(buff));
    printf("Message recieved at the client side after applying CRC is %s\n",buff);
    char res[BUF_SIZE];
    strcpy(res,crc(buff,div));
    
    int i,j;
    for(i=strlen(res) - 1 ,j=1 ; j < strlen(div) ; i--,j++)
    {
        if(res[i]=='1')
            break;
    }
    if(j<strlen(div))
        printf("There is error present in the message from server\n");
    else {
        printf("There is no error present in the message from server\n");
    }
    close(sockfd);
    return 0;
}


int server()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("Socket retrieve success\n");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

    if(listen(listenfd, 10) == -1)
    {
        printf("Failed to listen\n");
        return -1;
    }

    int connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);
    
    char buf[BUF_SIZE]={0};
    read(connfd, buf, sizeof(buf));
    char div[BUF_SIZE];
    strcpy(div,buf);
    RESET(buf);
    printf("Divisor recieved at server end is %s\n",div);
    
    char message[BUF_SIZE];
    rand_string(message,40);
    printf("Generated message is %s\n",message);
    char crc_message[BUF_SIZE];
    strcpy(crc_message,crc(message,div));
    printf("Message after applying crc on original message is %s\n",crc_message);
    write(connfd, crc_message, sizeof(buf));
    close(connfd);
    return 0;
}

int main(int argc, char** argv)
{
    if (argc==2)
    {
        const char* mode = argv[1];
        const char* filename = argv[2];
        if (strcmp(mode, "client") == 0)
            return client();
        else if (strcmp(mode, "server") == 0)
            return server();
        else
            printf("Invalid mode %s - should be 'client' or 'server'\n",mode);
    }
    else
    {
        printf("Invalid arguments.\n");
    }
    return 0;
}
