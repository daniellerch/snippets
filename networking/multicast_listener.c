/*
   Copyright (c) 2008 Daniel Lerch Hostalot <dlerch@gmail.com>

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
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>


#define PORT 9999
#define GROUP "225.0.0.37"

int main(int argc, char *argv[])
{
	struct sockaddr_in addr;
	int fd, nbytes;
	unsigned int addrlen;
	struct ip_mreq mreq;
	char msgbuf[256];
	u_int yes=1;

	if((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket");
		exit(1);
	}

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
				&yes, sizeof(yes)) < 0)
	{
		perror("setsockopt() SO_REUSEADDR");
		exit(1);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	addr.sin_port=htons(PORT);

	if(bind(fd,(struct sockaddr *) &addr,
				sizeof(addr)) < 0)
	{
		perror("bind");
		exit(1);
	}

	mreq.imr_multiaddr.s_addr=inet_addr(GROUP);
	mreq.imr_interface.s_addr=htonl(INADDR_ANY);

	if(setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
				&mreq, sizeof(mreq)) < 0)
	{
		perror("setsockopt");
		exit(1);
	}


	while(1)
	{
		addrlen=sizeof(addr);
		if ((nbytes=recvfrom(fd, msgbuf,
						sizeof(msgbuf), 0, (struct sockaddr *)
						&addr, &addrlen)) < 0)
		{
			perror("recvfrom");
			exit(1);
		}

		puts(msgbuf);
	}

	return 0;
}

