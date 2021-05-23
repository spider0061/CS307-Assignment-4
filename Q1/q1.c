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

int client()
{
    
    char filename1[BUF_SIZE];
    printf("Enter the filename to download\n");
    scanf("%s",&filename1);
    
    char filename[BUF_SIZE];
    printf("Enter filename in which you want to store the downloaded file\n");
    scanf("%s",&filename);
    
    /* Create file where data will be stored */
    FILE *fp = fopen(filename, "ab");
    if(NULL == fp)
    {
        printf("Error opening file");
        return 1;
    }

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

    /* Receive data in chunks of BUF_SIZE bytes */
    int bytesReceived = 0;
    char buff[BUF_SIZE];
    memset(buff, '0', sizeof(buff));
    
    write(sockfd, filename1, sizeof(buff));
    printf("%s",filename1);
    while((bytesReceived = read(sockfd, buff, BUF_SIZE)) > 0)
    {
        printf("Bytes received %d\n",bytesReceived);
        fwrite(buff, 1,bytesReceived,fp);
    }

    if(bytesReceived < 0)
    {
        printf("\n Read Error \n");
    }

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

    for (;;)
    {
        int connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);
        
        char buf[BUF_SIZE]={0};
        read(connfd, buf, sizeof(buf));
        printf("%s_ashutosh_testing\n",buf);
        
        char filename[BUF_SIZE];
        strcpy(filename,buf);
        
        /* Open the file that we wish to transfer */
        FILE *fp = fopen(filename,"rb");
        if(fp==NULL)
        {
            printf("File opern error");
            return 1;
        }

        /* Read data from file and send it */
        for (;;)
        {
            /* First read file in chunks of BUF_SIZE bytes */
            unsigned char buff[BUF_SIZE]={0};
            int nread = fread(buff,1,BUF_SIZE,fp);
            printf("Bytes read %d \n", nread);

            /* If read was success, send data. */
            if(nread > 0)
            {
                printf("Sending \n");
                write(connfd, buff, nread);
            }

            /*
             * There is something tricky going on with read ..
             * Either there was error, or we reached end of file.
             */
            if (nread < BUF_SIZE)
            {
                if (feof(fp))
                    printf("End of file\n");
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }
        }
        close(connfd);
        sleep(1);
    }

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
