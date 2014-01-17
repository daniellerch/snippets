/*
   Copyright (c) 2012 Daniel Lerch Hostalot <dlerch@gmail.com>

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




/*
	Compilation:
   gcc arp_simple_sniffer.c -lpcap
*/


#include <pcap.h> 
#include <string.h> 
#include <stdlib.h> 

#define MAX_BYTES 2048 

void packet_callback(
		u_char *arg, 
		const struct pcap_pkthdr* pkthdr, 
		const u_char *packet)
{ 
	int i=0;

	for(i=0; i<pkthdr->len; i++)
	{ 
		if(isprint(packet[i]))
			printf("%c ", packet[i]); 
		else 
			printf(". "); 
	}
} 



int main(int argc, char *argv[])
{ 
	int i=0; 
	pcap_t *pcap = NULL; 
	char errbuf[PCAP_ERRBUF_SIZE];
	char *dev = NULL; 

	memset(errbuf,0,PCAP_ERRBUF_SIZE); 

	if(argc!=2)
	{
		printf("Usage: %s <interface>\n", argv[0]);
		return 0;
	}

	dev=argv[1];

	if((pcap=pcap_open_live(dev, MAX_BYTES, 1,  512, errbuf)) == NULL)
	{
		fprintf(stderr, "Error: %s\n", errbuf);
		exit(1);
	}

	if(pcap_loop(pcap, -1, packet_callback, NULL) == -1)
	{
		fprintf(stderr, "Error: %s\n", pcap_geterr(pcap));
		exit(1);
	}

	return 0; 
} 

