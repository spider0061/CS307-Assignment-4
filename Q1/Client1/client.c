// client code for UDP socket programming
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define IP_Protocol 0
#define IP_Address "127.0.0.1" // localhost
#define Port_No 15050
#define Net_Buf_Size 32
#define CipherKey 'S'
#define SendRecvFlag 0
// function for clearing buffer
void clearBuf(char* b1){
   int i;
   for (i = 0; i < Net_Buf_Size; i++)
   b1[i] = '\0';
}
// function for decryption method
char Cipher(char ch1){
   return ch1 ^ CipherKey;
}
// function for receiveing file
int recvFile(char* buf1, int s1)
{
   int i;
   char ch1;
   for (i = 0; i < s1; i++) {
      ch1 = buf1[i];
      ch1 = Cipher(ch1);
      if (ch1 == EOF)
      return 1;
      else
      printf("%c", ch1);
   }
   return 0;
}

void fd_notreci() {
    printf("\nfile descriptor is not received!!\n");
}

void fd_reci(int i) {
    printf("\nfile descriptor %d is received\n", i);
}

void start() {
    int sockfd1, nBytes;
    struct sockaddr_in addr_con;
    int addrlen = sizeof(addr_con);
    addr_con.sin_family = AF_INET;
    addr_con.sin_port = htons(Port_No);
    addr_con.sin_addr.s_addr = inet_addr(IP_Address);
    char net_buf1[Net_Buf_Size];
    FILE* fp1;
    // socket()
    sockfd1 = socket(AF_INET, SOCK_DGRAM,
    IP_Protocol);
    if (sockfd1 < 0)
        fd_notreci;
    else
        fd_reci(sockfd1);
    while (1) {
       printf("\nPlease enter the name of file to receive:\n");
       scanf("%s", net_buf1);
       sendto(sockfd1, net_buf1, Net_Buf_Size,
       SendRecvFlag, (struct sockaddr*)&addr_con,
       addrlen);
       printf("\nThe contents of the file are :->\n");
       while (1) {
          // receive
          clearBuf(net_buf1);
          nBytes = recvfrom(sockfd1, net_buf1, Net_Buf_Size,
          SendRecvFlag, (struct
          sockaddr*)&addr_con,
          &addrlen);
          // process
          if (recvFile(net_buf1, Net_Buf_Size)) {
             break;
          }
       }
       printf("\n______________________________________________________________\n");
    }
}
// driver code
int main(){
    start();
   return 0;
}
