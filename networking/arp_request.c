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
#include <getopt.h>
#include <errno.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <malloc.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <net/if.h>
#include <sys/ioctl.h>



struct arp_hdr 
{
	unsigned short int hardware;
	unsigned short int protocol;
	char hw_addr_len;  
	char proto_addr_len;
	unsigned short operation;
	char src_addr[6];
	char src_ip[4];
	char dst_addr[6];
	char dst_ip[4];
};


int main(int argc, char *argv[]) 
{
	if(argc!=3)
	{
		printf("Usage: %s <iface> <ip>\n", argv[0]);
		return 0;
	}

	/* Source and destination mac address */
	char src_mac[6];
	char dst_mac[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	
	/* Direcciones IP */
	char src_ip[4];
	char dst_ip[4];

	int a, b, c, d;
	sscanf(argv[2], "%d.%d.%d.%d", &a, &b, &c, &d);
	dst_ip[0]=a;
	dst_ip[1]=b;
	dst_ip[2]=c;
	dst_ip[3]=d;

	int sock;
	struct ifreq if_data;

	unsigned int buffer_size = 
	sizeof(struct arp_hdr) + sizeof(struct ether_header);

	unsigned char buffer[buffer_size];
	memset(buffer,0,buffer_size);

	struct ether_header *eth = (struct ether_header *)buffer;

	struct arp_hdr *arp = 
		(struct arp_hdr *)(buffer + sizeof(struct ether_header));

	/* Device */
	char dev[8];
	strncpy(dev, argv[1], 8);
	strcpy (if_data.ifr_name, dev);

	if((sock = socket(AF_INET, SOCK_PACKET, htons(ETH_P_ARP)))==-1) 
	{ 
		perror("socket()"); 
		exit(EXIT_FAILURE); 
	}

	/* Read local MAC address (source) */
	if(ioctl(sock, SIOCGIFHWADDR, &if_data)<0)
	{
		perror ("ioctl(): SIOCGIFHWADDR \n");
		exit(EXIT_FAILURE);
	}

	memcpy(src_mac, if_data.ifr_hwaddr.sa_data, 6);

	/* Read local IP address (Source) */
	if(ioctl(sock, SIOCGIFADDR, &if_data)<0)
	{
		perror ("ioctl(); SIOCGIFADDR \n");
		exit(EXIT_FAILURE);
	}
	
	u_int32_t ip;
	memcpy(src_ip, (void *) &if_data.ifr_addr.sa_data + 2, 4);


	/* Ethernet header */
	memcpy(eth->ether_dhost, dst_mac, ETHER_ADDR_LEN);
	memcpy(eth->ether_shost, src_mac, ETHER_ADDR_LEN);
	eth->ether_type = htons(ETHERTYPE_ARP);

	/* ARP header */
	arp->hardware = htons(ARPHRD_ETHER);
	arp->protocol = htons(ETH_P_IP);
	arp->hw_addr_len = 6;  	
	arp->proto_addr_len = 4;
	arp->operation = htons(ARPOP_REQUEST);
	memcpy(arp->src_addr, src_mac,6);
	memcpy(arp->src_ip, src_ip, 4);
	memcpy(arp->dst_addr, dst_mac, 6);
	memcpy(arp->dst_ip, dst_ip, 4);

	struct sockaddr addr;
	strncpy(addr.sa_data, dev, sizeof(addr.sa_data));

	/* Send ARP packet */	
	if ((sendto(sock, buffer, buffer_size, 0, 
		&addr, sizeof(struct sockaddr)))==-1) 
	{

		perror("sendto()");
		exit(EXIT_FAILURE);
	}

	return 0;
}

