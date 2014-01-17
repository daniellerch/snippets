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
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#define LISTEN_PORT 9999
#define BUFFER_SIZE 1024

int main() 
{
	int socket_descriptor;

	int port = LISTEN_PORT;

	struct sockaddr_in sin; 

	socket_descriptor = socket (AF_INET, SOCK_STREAM, 0);
	if (socket_descriptor == -1) 
	{
		perror("socket()");
		exit(EXIT_FAILURE);
	}

	bzero (&sin, sizeof(sin));
	sin.sin_family =  AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons (port);

	if (bind(socket_descriptor,(struct sockaddr *)&sin,sizeof(sin)) == -1) 
	{ 
		perror("bind()");  
		exit(EXIT_FAILURE);
	}

	if (listen(socket_descriptor, 20) == -1) 
	{
		perror("listen()");  
		exit(EXIT_FAILURE);
	}


	struct sockaddr_in pin;
	int address_size;

	int temp_socket_descriptor;

	char buffer[BUFFER_SIZE];

	while (1) 
	{
		/* Limpieza del buffer */
		memset (buffer, '\0', BUFFER_SIZE);

		temp_socket_descriptor = 
			accept(socket_descriptor,(struct sockaddr*)&pin,&address_size);

		if (temp_socket_descriptor == -1) 
		{ 
			perror("accept()");  
			exit(EXIT_FAILURE);
		}

		if (recv(temp_socket_descriptor,buffer,sizeof(buffer),0)==-1) 
		{ 
			perror("recv()");  
			exit(EXIT_FAILURE);
		}

		printf ("msg from client: %s\n",buffer); 

		if (send(temp_socket_descriptor, "Hi from server\n", 15,0) == -1) 
		{ 
			perror("send()");  
			exit(EXIT_FAILURE);
		}

		close(temp_socket_descriptor);
	}
}

