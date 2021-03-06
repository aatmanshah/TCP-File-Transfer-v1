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
		printf ("Usage: %s <ip of server> <port #> <input file name> <output file name> \n",argv[0]);
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
	serv_addr.sin_port = htons (atoi(argv[2]));

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
    printf("[Client] Sending to the Server... %s \n", argv[3]);

	instream = fopen(argv[3], "rb");   //binary read mode for fopen
    if(instream == NULL)
    {
        printf("open read file error.\n");
        return 0;
    }


    //send file name
    int file_name_size;
    write (sockfd, argv[4], strlen (argv[4]) + 1);


    //send file contents
    int send_block_size;
    while((send_block_size = fread(sdbuff, sizeof(char), BUFF_SIZE, instream)) > 0)
    {
        if(write(sockfd, sdbuff, send_block_size) < 0)
        {
            fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", argv[2], errno);
            break;
        }
        bzero(sdbuff, BUFF_SIZE);
    }
    printf("File %s from Client was sent successfully!\n", argv[3]);

    //close connection
	close (sockfd);
	return 0;
}
