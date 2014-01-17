/*
   Copyright (c) 2004 Daniel Lerch Hostalot <dlerch@gmail.com>

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
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <linux/ip.h>
#include <linux/icmp.h>
 


// {{{ checksum()
unsigned short checksum(unsigned short *addr, int len)
{
	register int sum = 0;
	u_short answer = 0;
	register u_short *w = addr;
	register int nleft = len;

	while (nleft > 1)
	{
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1)
	{
		*(u_char *) (&answer) = *(u_char *) w;
		sum += answer;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}
// }}}

// {{{ get_local_ip()
char* get_local_ip()
{
	char buffer[256];
    struct hostent* h;
     
    gethostname(buffer, 256);
    h = gethostbyname(buffer);
     
    return inet_ntoa(*(struct in_addr *)h->h_addr);
}
// }}}

int main(int argc, char *argv[])
{
	if(argc!=3)
	{
		printf("Usage: %s <ip> <request|reply>\n", argv[0]);
		return 0;
	}

	int sock;

	unsigned int buffer_size = sizeof(struct iphdr) + sizeof(struct icmphdr);

	unsigned char buffer[256];
	memset(buffer, 0, sizeof(buffer));

	struct iphdr *ip = (struct iphdr *)buffer;

	struct icmphdr *icmp = (struct icmphdr *)(buffer + sizeof(struct iphdr));

	int i;
	for(i=0; i<56; i++)
		buffer[buffer_size++]=i;

	if ((sock = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP)) == -1)
	{
		perror("socket()");
		exit(EXIT_FAILURE);
	}

	int o = 1;
	if( setsockopt(sock,IPPROTO_IP,IP_HDRINCL,&o,sizeof(o)) == -1 )
	{
		perror("setsockopt()");
		exit(EXIT_FAILURE);
	}

	ip->version=4;
	ip->ihl=5;
	ip->tos=0;
	ip->tot_len=sizeof(struct iphdr) + sizeof(struct icmphdr);
	ip->id=0;
	ip->frag_off=0;
	ip->ttl= 64;
	ip->protocol=IPPROTO_ICMP;
	ip->saddr=inet_addr(get_local_ip());
	ip->daddr=inet_addr(argv[1]);
	ip->check=checksum((unsigned short *)ip, sizeof(struct iphdr));

	if(strcmp(argv[2], "request")==0)
		icmp->type=ICMP_ECHO;
	else
		icmp->type=ICMP_ECHOREPLY;

	icmp->code=0;
	icmp->un.echo.id=0;
	icmp->un.echo.sequence=0;
	icmp->checksum=checksum((unsigned short *)icmp, buffer_size);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;

	if ((sendto(sock, buffer, buffer_size, 0, (struct sockaddr*)&addr,
					sizeof(struct sockaddr_in))) == -1 )
	{
		perror("send()");
		exit(EXIT_FAILURE);
	}

	return 0;
}
