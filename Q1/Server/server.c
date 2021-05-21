// Server Code
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define IP_Protocol 0
#define Port_No 15050
#define Net_Buf_Size 32
#define CipherKey 'S'
#define SendRecvFlag 0
#define NoFile "File Not Found!"


// function for clearing buffer
void clearBuf(char* b1){
   int i;
   for (i = 0; i < Net_Buf_Size; i++)
   b1[i] = '\0';
}
// function for encryption method

char Cipher(char ch1){
   return ch1 ^ CipherKey;
}
// function for sending file
int sendFile(FILE* fp1, char* buf1, int s1){
   int i, len;
   if (fp1 == NULL) {
      strcpy(buf1, NoFile);
      len = strlen(NoFile);
      buf1[len] = EOF;
      for (i = 0; i <= len; i++)
      buf1[i] = Cipher(buf1[i]);
      return 1;
   }
   char ch1, ch2;
   for (i = 0; i < s1; i++) {
      ch1= fgetc(fp1);
      ch2 = Cipher(ch1);
      buf1[i] = ch2;
      if (ch1 == EOF)
      return 1;
   }
   return 0;
}

void fd_notreci() {
    printf("\nfile descriptor is not received!!\n");
}

void fd_reci(int i) {
    printf("\nfile descriptor %d is received\n", i);
}

void suc_binded() {
    printf("\nSuccessfully is binded!\n");
}

void not_suc_binded() {
    printf("\nBinding is Failed!\n");
}

void start() {
    int sockfd1, nBytes;
    struct sockaddr_in addr_con;
    int addrlen = sizeof(addr_con);
    addr_con.sin_family = AF_INET;
    addr_con.sin_port = htons(Port_No);
    addr_con.sin_addr.s_addr = INADDR_ANY;
    char net_buf1[Net_Buf_Size];
    FILE* fp1;
    // socket()
    sockfd1 = socket(AF_INET, SOCK_DGRAM, IP_Protocol);
    if (sockfd1 < 0)
        fd_notreci();
    else
        fd_reci(sockfd1);
    // bind()
    if (bind(sockfd1, (struct sockaddr*)&addr_con,
       sizeof(addr_con)) == 0)
        suc_binded();
    else
        not_suc_binded();
    while (1) {
       printf("\nWaiting for name of file...\n");
       // receive name of file
       clearBuf(net_buf1);
       nBytes = recvfrom(sockfd1, net_buf1,
       Net_Buf_Size, SendRecvFlag,
       (struct sockaddr*)&addr_con,
       &addrlen);
       fp1 = fopen(net_buf1, "r");
       printf("\nFile Name is Received: %s\n", net_buf1);
       if (fp1 == NULL)
       printf("\nFile open is failed!\n");
       else
       printf("\nFile Successfully is opened!\n");
       while (1) {
          // process
          if (sendFile(fp1, net_buf1, Net_Buf_Size)) {
             sendto(sockfd1, net_buf1, Net_Buf_Size,
             SendRecvFlag,
             (struct sockaddr*)&addr_con,
             addrlen);
             break;
          }
          // send
          sendto(sockfd1, net_buf1, Net_Buf_Size,
          SendRecvFlag,
          (struct sockaddr*)&addr_con, addrlen);
          clearBuf(net_buf1);
       }
       if (fp1 != NULL)
       fclose(fp1);
    }
}

// driver code
int main(){
    start();
    return 0;
}
