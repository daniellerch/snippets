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
	if(argc!=4)
	{
		printf("Usage: %s <iface> <ip> <mac>\n", argv[0]);
		return 0;
	}

	/* Source and destination mac address */
	char src_mac[6];
	char dst_mac[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	
	/* Direcciones IP */
	char src_ip[4];
	char dst_ip[] = {0, 0, 0, 0};

	int a, b, c, d, e, f;
	sscanf(argv[2], "%d.%d.%d.%d", &a, &b, &c, &d);
	src_ip[0]=a;
	src_ip[1]=b;
	src_ip[2]=c;
	src_ip[3]=d;

	sscanf(argv[3], "%x:%x:%x:%x:%x:%x", &a, &b, &c, &d, &e, &f);
	src_mac[0]=a;
	src_mac[1]=b;
	src_mac[2]=c;
	src_mac[3]=d;
	src_mac[4]=e;
	src_mac[5]=f;


	int sock;

	unsigned int buffer_size = 
	sizeof(struct arp_hdr) + sizeof(struct ether_header)+18;

	unsigned char buffer[buffer_size];
	memset(buffer,0,buffer_size);

	struct ether_header *eth = (struct ether_header *)buffer;

	struct arp_hdr *arp = 
		(struct arp_hdr *)(buffer + sizeof(struct ether_header));

	/* Device */
	char dev[8];
	strncpy(dev, argv[1], 8);

	if((sock = socket(AF_INET, SOCK_PACKET, htons(ETH_P_ARP)))==-1) 
	{ 
		perror("socket()"); 
		exit(EXIT_FAILURE); 
	}

	/* Ethernet header */
	memcpy(eth->ether_dhost, dst_mac, ETHER_ADDR_LEN);
	memcpy(eth->ether_shost, src_mac, ETHER_ADDR_LEN);
	eth->ether_type = htons(ETHERTYPE_ARP);

	/* ARP header */
	arp->hardware = htons(ARPHRD_ETHER);
	arp->protocol = htons(ETH_P_IP);
	arp->hw_addr_len = 6;  	
	arp->proto_addr_len = 4;
	arp->operation = htons(ARPOP_REPLY);
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

