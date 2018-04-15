 /**************************
 *     socket example, server
 *     spring 2018
 ***************************/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#define _POSIX_SOURCE
#define BUFF_SIZE 10
#define PORT 3500

//int fileno(const FILE *stream);

int main (int, char *[]);


/*********************
 * main
 *********************/
int main (int argc, char *argv[])
{
	int	n;
	char *p;
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	char buff[1025];
	char filename[32];
	FILE *outstream;
	char sdbuff[BUFF_SIZE];

	// set up
	memset (&serv_addr, '0', sizeof (serv_addr));
	memset (buff, '0', sizeof (buff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	serv_addr.sin_port = htons (5000);

	// create socket, bind, and listen
	listenfd = socket (AF_INET, SOCK_STREAM, 0);
	bind (listenfd, (struct sockaddr*)&serv_addr, sizeof (serv_addr));
	listen (listenfd, 10);

	// accept and interact
	while (1)
	{
		connfd = accept (listenfd, (struct sockaddr*)NULL, NULL);

		// receive data and reply
		while ((n = read (connfd, buff, sizeof (buff))) > 0)
		{
		    //receive file name
		    size_t destination_size = sizeof(buff);

		    strncpy(filename, buff, destination_size);
		    /*
			// change lower to upper case
			p = buff;
			while (*p != '\0')
			{
				*p = toupper (*p);
				p++;
			}

			write (connfd, buff, p - buff + 1);
			*/
		}

		//OPEN (CREATE) DESTINATION FILE
		outstream = fopen(filename, "a+w");  //for fwrite
        if (outstream == NULL)
        {
            printf("open dest file error.\n");
            return 0;
        }

		//RECEIVE FILE DATA AND WRITE TO OUTPUT FILE
		int i;
		while((i = read (connfd, sdbuff, sizeof (sdbuff))) > 0)
        {
            write(fileno(outstream), sdbuff, sizeof(sdbuff) + 1);
        }

        fclose(outstream);
		close (connfd);
	}
}
