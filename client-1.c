 /**************************
	socket example, client
	spring 2018
 **************************/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#define BUFF_SIZE 10    //SIZE OF RECEIVE BUFFER
#define PORT 3500

int main (int, char *[]);


/********************
 * main
 ********************/
int main (int argc, char *argv[])
{
	int i;
	int sockfd = 0, n = 0;
	char buff[1024];
	char *p;
	struct sockaddr_in serv_addr;
	int file_size;
	FILE *instream;
	int data_remaining = 0;

	if (argc != 5)
	{
		printf ("Usage: %s <ip of server> \n",argv[0]);
		return 1;
	}

	// set up
	memset (buff, '0', sizeof (buff));
	memset (&serv_addr, '0', sizeof (serv_addr));

	// open socket
	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf ("Error : Could not create socket \n");
		return 1;
	}

	// set address
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons (5000);

	if (inet_pton (AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
	{
		printf ("inet_pton error occured\n");
		return 1;
	}

	// connect
	if (connect (sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr)) < 0)
	{
		printf ("Error : Connect Failed \n");
		return 1;
	}


	//open file
	char sdbuff[BUFF_SIZE];
    printf("[Client] Sending to the Server... %s ", argv[2]);

	instream = fopen(argv[2], "rb");   //binary read mode for fopen
    if(instream == NULL)
    {
        printf("open read file error.\n");
        return 0;
    }

    //bzero(sdbuff, BUFF_SIZE);

    //send file name
    int file_name_size;
    while((file_name_size = read(sockfd, argv[2], 32) > 0))
    {
        write (sockfd, buff, strlen (buff) + 1);
        /*
        if(send(sockfd, sdbuff, file_name_size, 0) < 0)
        {
            fprintf(stderr, "ERROR: Failed to send file name %s. (errno = %d)\n", argv[2], errno);
            break;
        }
        */
        bzero(sdbuff, BUFF_SIZE);
    }

    int send_block_size;
    while((send_block_size = fread(sdbuff, sizeof(char), BUFF_SIZE, instream)) > 0)
    {
        if(send(sockfd, sdbuff, send_block_size, 0) < 0)
        {
            fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", argv[2], errno);
            break;
        }
        bzero(sdbuff, BUFF_SIZE);
    }
    printf("File %s from Client was sent successfully!\n", argv[2]);

	while (scanf ("%s", buff) == 1)
	{
		write (sockfd, buff, strlen (buff) + 1);
		read (sockfd, buff, sizeof (buff));
		printf ("%s\n", buff);
	}



	close (sockfd);

	return 0;
}
