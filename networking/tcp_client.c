/*
   Copyright (c) 2002 Daniel Lerch Hostalot <dlerch@gmail.com>

   Permission is hereby granted, free of charge, to any person obtaining a 
   copy of this software and associated documentation files (the "Software"), 
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense, 
   and/or sell copies of the Software, and to permit persons to whom the 
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in 
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
   DEALINGS IN THE SOFTWARE.
*/



#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
	if(argc!=4)
	{
		printf("Usage: %s <host> <port> <msg>\n", argv[0]);
		return 0;
	}

	char *host = argv[1];
	int port = atoi(argv[2]);
	char *data = argv[3];

	struct hostent *host_name;
	if ((host_name = gethostbyname(host))==0) 
	{
		perror ("gethostbyname()");
		exit (EXIT_FAILURE);
	}

	struct sockaddr_in pin; 
	bzero (&pin, sizeof(pin));
	pin.sin_family =  AF_INET;
	pin.sin_addr.s_addr = htonl(INADDR_ANY);
	pin.sin_addr.s_addr = ((struct in_addr *)(host_name->h_addr))->s_addr;
	pin.sin_port = htons (port);


	int socket_descriptor = socket (AF_INET, SOCK_STREAM, 0);
	if (socket_descriptor == -1) 
	{ 
		perror ("socket()");
		exit (EXIT_FAILURE);
	}


	if (connect(socket_descriptor, (void *)&pin, sizeof(pin))==-1) 
	{
		perror ("connect()");
		exit (EXIT_FAILURE);
	}

	if (send(socket_descriptor, data, strlen(data), 0) == -1) 
	{
		perror ("send()");
		exit (EXIT_FAILURE);
	}

	static char buffer [2048];
	if (recv(socket_descriptor, buffer, sizeof(buffer), 0) == -1) {

		perror ("recv()");
		exit (EXIT_FAILURE);
	}

	printf ("server msg: %s\n", buffer);
}

