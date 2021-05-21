#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define RESET(buff) for(i=0;i<100;i++) buff[i]='\0';

void error(const char *msg) {
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
        printf("%s\n",temp);
        printf("%s\n",rem);
    }

    // Reduce remainder
    for (int i = 0; i < key_len; i++)
    {
        rem[i] = rem[i+1];
    }
    return strcat(msg,rem);
}

int main(int argc, char const *argv[]) {
int sockfd, i,j;
struct sockaddr_in serv_addr;
char buff[100], key[100], data[100];
char *res=malloc(30);
    
//Checking the provided port no
if (argc < 3)
    error("Port No not provided");
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
serv_addr.sin_port = htons(atoi(argv[2]));

//Socket
if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
error("Error in Socket");
    
//Connect
if( connect(sockfd , (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
error("Error in connecting");

//sending the data
RESET(buff)
recv(sockfd , buff, 100, 0);
printf(" %s ",buff);
scanf("%s",data);
RESET(buff)
strcpy(buff,data);
send(sockfd , buff, 100, 0);

//Sending the key
RESET(buff)
recv(sockfd, buff, 100, 0);
printf("%s",buff);
scanf("%s",key);
RESET(buff)
strcpy(buff,key);
send(sockfd, buff, 100, 0);

//Get the crc from server
RESET(buff)
recv(sockfd , buff, 100, 0);
printf("From The server : %s \n",buff);

//Check the crc at receiver side
res = crc(buff,key);
printf("\nAt the receiver End : \n");
for(i=0 ; res[i] != '\0' ; i++)
printf("%c",res[i]);
for(i=strlen(res) - 1 ,j=1 ; j < strlen(key) ; i--,j++)
{
if(res[i]=='1')
break;
}
if(j<strlen(key))
    send(sockfd, "Error", 100, 0);
else {
    send(sockfd, "OKAY ",100,0);
}
close(sockfd);
printf("\n");
return 0 ;
}
