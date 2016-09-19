#include "packetEngine.h"
#include "error.h"
#include "linuxsyscall.h"

FILE *logfile;
FILE *loghdr;

extern int stopThreads;
int sock_row;
int tcp = 0 , udp= 0, icmpp = 0, igmp = 0, raw = 0,stcp=0, others = 0,total = 0;
struct sockaddr_in source, dest;

void print_ethernet_header(const u_char *packet, int size)
{
    struct ethhdr *eth = (struct ethhdr *)packet;

    fprintf(logfile, "\n");
    fprintf(logfile, "Ethernet Header\n");
    fprintf(logfile, "      |-Destination address       : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
    fprintf(logfile, "      |-Source address            : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
    fprintf(logfile, "      |-Protocol                  : %u \n",(unsigned short) eth->h_proto);
    fflush(logfile);
}

void getDstMacAddr(const u_char *packet, char *dMac)
{
	struct ethhdr *eth = (struct ethhdr *)packet;

	snprintf(dMac, 49, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
}

void getSrcMacAddr(const u_char *packet, char *sMac)
{
        struct ethhdr *eth = (struct ethhdr *)packet;

        snprintf(sMac, 49, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
}

void getMacAddr(const u_char *packet, char *dMac, char *sMac)
{
	 struct ethhdr *eth = (struct ethhdr *)packet;

        snprintf(dMac, 49, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
	snprintf(sMac, 49, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
}

void print_ip_header(const u_char *packet, int size)
{
  //  print_ethernet_header(packet, size);

    unsigned short iphdrlen;
    struct hostent *hp;

    struct iphdr *iph = (struct iphdr *)(packet + sizeof(struct ethhdr));
    iphdrlen  = iph->ihl * 4;

    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
    source.sin_family = AF_INET;

    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
    dest.sin_family = AF_INET;

    char shost[1024], dhost[1024];
    char sservice[20], dservice[20];

	// use getaddrinfo
    int x = getnameinfo((struct sockaddr *)&source, sizeof(source),shost, sizeof(shost),sservice, sizeof(sservice) , 0);
    if (x)
    {
       	errMsg("getnameinfo error, failed to get name source info");
	shost[0] = 0;
    }
    
    x = getnameinfo((struct sockaddr *)&dest, sizeof(dest),dhost, sizeof(dhost),dservice, sizeof(dservice) , 0);
    if (x)
    {
       errMsg("getnameinfo error, failed to get name dest info");
       dhost[0] = 0;
    }

	char *sIp = (char *)malloc(18);
	
    fprintf(logfile , "\n");
    fprintf(logfile , "IP Header\n");
    fprintf(logfile, "      | -IP Version               : %d\n", (unsigned int) iph->version);
    fprintf(logfile, "      | -IP Header Length         : %d DWORDS or %d Bytes\n", iph->ihl , iph->ihl * 4);
    fprintf(logfile, "      | -Type of Services         : %d\n", (unsigned int)iph->tos);
    fprintf(logfile,"       | -IP Total Length         : %d\n", ntohs(iph->tot_len) );
    fprintf(logfile,"       | -Identification          : %d\n", ntohs(iph->id));
    //fprintf(logfile , "   |-Reserved ZERO Field   : %d\n",(unsigned int)iphdr->ip_reserved_zero);
        //fprintf(logfile , "   |-Dont Fragment Field   : %d\n",(unsigned int)iphdr->ip_dont_fragment);
        //fprintf(logfile , "   |-More Fragment Field   : %d\n",(unsigned int)iphdr->ip_more_fragment);
    fprintf(logfile,"       | -TTL                     : %d\n", (unsigned int)iph->ttl);
    fprintf(logfile,"       | -Protocol                : %d\n", (unsigned int)iph->protocol);
    fprintf(logfile,"       | -Checksum                : %d\n", ntohs(iph->check));
    fprintf(logfile,"       | -Source IP               : %s ( host : %s)\n", getSrcIp(packet, sIp) /*inet_ntoa(source.sin_addr)*/, shost);
    fprintf(logfile,"       | -Destination IP          : %s ( host : %s)\n", inet_ntoa(dest.sin_addr), dhost);
	free(sIp);
}

char * getSrcIp(const u_char *packet, char *sIp)
{
	if(!sIp)
	{
		
		return NULL;
	}
	struct iphdr *iph = (struct iphdr *) (packet + sizeof(struct ethhdr));
	struct sockaddr_in source;

	bzero(&source, sizeof(struct sockaddr_in));
	source.sin_addr.s_addr = iph->saddr;
	
	snprintf(sIp, 16, "%s", inet_ntoa(source.sin_addr));
return sIp;
}

char * getDestIp(const u_char *packet, char *dIp)
{
	if(!dIp)	
		return NULL;

        struct iphdr *iph = (struct iphdr *) (packet + sizeof(struct ethhdr));
        struct sockaddr_in dest;

        bzero(&dest, sizeof(struct sockaddr_in));

 	dest.sin_addr.s_addr = iph->daddr;

        snprintf(dIp, 16, "%s", inet_ntoa(dest.sin_addr));
return dIp;
}

char *getProtocol(const u_char *packet, char *protocol)
{
	struct iphdr *iph = (struct iphdr*) (packet + sizeof(struct ethhdr));

	snprintf(protocol, 12, "%d", iph->protocol);
return protocol;
}

int getSrcPort(const u_char *packet)
{
	struct tcphdr *tcphdr = (struct tcphdr *)(packet + sizeof(struct iphdr) + sizeof(struct ethhdr));

return htons(tcphdr->source);
}

int getDestPort(const u_char *packet)
{
        struct tcphdr *tcphdr = (struct tcphdr *)(packet + sizeof(struct iphdr) + sizeof(struct ethhdr));

return htons(tcphdr->dest);
}


void print_tcp_packet(const u_char *packet, int size)
{
    unsigned short iphdrlen = 0;

    struct iphdr *iph = (struct iphdr *)(packet);// + sizeof(struct ethhdr));
    iphdrlen = iph->ihl*4; // ihl need to be 5

//    struct tcphdr *tcphdr = (struct tcphdr *)(packet + sizeof(struct ether_header) + sizeof(struct iphdr));
    struct tcphdr *tcphdr = (struct tcphdr *)(packet );//+ sizeof(struct ethhdr) + iphdrlen);

  //  int heder_size = sizeof(struct ethhdr ) + iphdrlen + tcphdr->doff*4;

    fprintf(logfile , "\n\n***********************TCP Packet*************************\n");
    print_ip_header(packet, size);

    fprintf(logfile , "\n");
    fprintf(logfile , "TCP Header\n");
    fprintf(logfile, "      | -Source Port                              : %d\n", htons(tcphdr->source));
    fprintf(logfile, "      | -Destination Port                     : %d\n", htons(tcphdr->dest));
    fprintf(logfile, "      | -Sequence number                  : %d\n",  htons(tcphdr->seq));
    fprintf(logfile, "      | -Acknowlidge number            : %d\n", htons(tcphdr->ack_seq));
    fprintf(logfile, "      | -Header Length                        : %d DWORD or %d Bytes \n",(unsigned int)tcphdr->doff, (unsigned int)tcphdr->doff*4 );
    //fprintf(logfile , "   |-CWR Flag : %d\n",(unsigned int)tcph->cwr);
      //fprintf(logfile , "   |-ECN Flag : %d\n",(unsigned int)tcph->ece);
    fprintf(logfile, "      | -Urgent flag                              : %d\n", (unsigned int ) tcphdr->urg);
    fprintf(logfile, "      | -Acknowlidgment Flag          : %d\n", (unsigned int ) tcphdr->ack);
    fprintf(logfile, "      | -Push Flag                                  : %d\n", (unsigned int ) tcphdr->psh);
    fprintf(logfile, "      | -Reset Flag                                : %d\n", (unsigned int ) tcphdr->rst);
    fprintf(logfile, "      | -Synchronise Port                    : %d\n", (unsigned int ) tcphdr->syn);
    fprintf(logfile, "      | -Finish Port                                : %d\n", (unsigned int ) tcphdr->fin);
    fprintf(logfile, "      | -Window                                     : %d\n", ntohs(tcphdr->window));
    fprintf(logfile, "      | -CheckSu                                    : %d\n", ntohs(tcphdr->check));
    fprintf(logfile, "      | -Urgent Pointer                       : %d\n", ntohs(tcphdr->urg_ptr));
    fprintf(logfile , "\n");
    fprintf(logfile , "                        DATA Dump                         ");
    fprintf(logfile , "\n");

    fprintf(logfile, "IP Header\n");
    PrintData(packet,iphdrlen);

    fprintf(logfile, "TCP Header\n");
    PrintData(packet+iphdrlen, tcphdr->doff*4);

    fprintf(logfile, "Data Payload\n");
    PrintData(packet + iphdrlen + tcphdr->doff*4 , (size - tcphdr->doff*4-iph->ihl*4) );

    fprintf(logfile , "\n###########################################################");
    fflush(logfile);
}

void print_udp_packet(const u_char *Buffer , int Size)
{

    unsigned short iphdrlen;

    struct iphdr *iph = (struct iphdr *)(Buffer );//+  sizeof(struct ethhdr));
    iphdrlen = iph->ihl*4;

    struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen  );//+ sizeof(struct ethhdr));

  //  int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof udph;

    fprintf(logfile , "\n\n***********************UDP Packet*************************\n");

    print_ip_header(Buffer,Size);

    fprintf(logfile , "\nUDP Header\n");
    fprintf(logfile , "   |-Source Port      : %d\n" , ntohs(udph->source));
    fprintf(logfile , "   |-Destination Port : %d\n" , ntohs(udph->dest));
    fprintf(logfile , "   |-UDP Length       : %d\n" , ntohs(udph->len));
    fprintf(logfile , "   |-UDP Checksum     : %d\n" , ntohs(udph->check));

    fprintf(logfile , "\n");
    fprintf(logfile , "IP Header\n");
    PrintData(Buffer , iphdrlen);

    fprintf(logfile , "UDP Header\n");
    PrintData(Buffer+iphdrlen , sizeof udph);

    fprintf(logfile , "Data Payload\n");

    //Move the pointer ahead and reduce the size of string
    //PrintData(Buffer + header_size , Size - header_size);
    PrintData(Buffer + iphdrlen + sizeof udph ,( Size - sizeof udph - iph->ihl * 4 ));

    fprintf(logfile , "\n###########################################################");
}

void PrintData(const u_char * packet, int size)
{
        int i, j;

        for(i = 0 ;  i <  size ; i++)
        {
            if(i != 0  && i  % 16 == 0)
            {
                 fprintf(logfile , "         ");
                 for(j = i - 16 ; j < i ; j++)
                 {
                     if(packet[j] >=32 && packet[j]  <= 128)
                         fprintf(logfile, "%c", (unsigned char) packet[j]);
                     else
                         fprintf(logfile, ".");
                 }

                 fprintf(logfile, "\n");
            }

            if(i % 16 == 0)
                fprintf(logfile, "      ");
            fprintf(logfile, " %02X", (unsigned int)packet[i]);

            if( i==size-1)  //print the last spaces
                   {
                       for(j=0;j<15-i%16;j++)
                       {
                         fprintf(logfile , "   "); //extra spaces
                       }

                       fprintf(logfile , "         ");

                       for(j=i-i%16 ; j<=i ; j++)
                       {
                           if(packet[j]>=32 && packet[j]<=128)
                           {
                             fprintf(logfile , "%c",(unsigned char)packet[j]);
                           }
                           else
                           {
                             fprintf(logfile , ".");
                           }
                       }

                       fprintf(logfile ,  "\n" );
                   }
        }
}

void processPacket(u_char * packet, int size)
{
    struct iphdr *iph = (struct iphdr*)(packet + sizeof( struct ethhdr));
    ++total;

    switch (iph->protocol)
    {
                case IPPROTO_IP:
                    break;
                case IPPROTO_EGP:
                    break;

                case IPPROTO_ICMP:
                                ++icmpp;

                    break;
                case IPPROTO_IGMP:
                                ++igmp;

                    break;

                case IPPROTO_TCP:
                                  ++tcp;
        //            print_tcp_packet(packet, size);
                    break;
               case IPPROTO_UDP:
                                    ++udp;
          //          print_udp_packet(packet, size);

                    break;

                case IPPROTO_GRE:
                    break;
                case IPPROTO_SCTP :
                        ++stcp;
                break;

                case IPPROTO_RAW:
                    ++raw;
                    break;

                default:
                    ++others;
        break;
    }
	char *srcIp = (char *)malloc(18);
	char *dstIp = (char *) malloc(18);
	char *protocol = (char *)malloc(10);
	fprintf(loghdr, "src ip = %s : port = %d : dst ip = %s : port = %d : PROTO = %s\n", getSrcIp(packet, srcIp), getSrcPort(packet), getDestIp(packet, dstIp), getDestPort(packet), getProtocol(packet, protocol));
	free(srcIp);
	free(dstIp);
	free(protocol);
fprintf(stderr, "TCP : %d   UDP : %d    ICMP : %d   IGMP : %d STCP : %d  Raw :  %d   Others : %d  Total : %d\r", tcp, udp, icmpp, igmp, stcp, raw, others, total);
}

void initSnifferRaw(void)
{
                int saddr_size, data_size;
                struct sockaddr saddr;
                struct in_addr   in;

                unsigned char *buffer = (unsigned char *) malloc(65536);

                logfile = fopen("log.txt", "w");
                if(!logfile) 
		{
			errQuit("Unable to open file\n");
		}

		loghdr = fopen("loghdr.txt", "a");
		if(!loghdr)
		{
			errQuit("unable to open file\n");
		}
                fprintf(stderr, "Starting....\n");

//               sock_row = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
		sock_row = socket( AF_PACKET , SOCK_RAW , htons(ETH_P_ALL)) ;
                if(sock_row < 0 )
                {
                        errQuit("Socket error");
                        //return;
                }
		fprintf(stderr, "Entering while(1)\n");
                while(1)
                {
			if(stopThreads)
				break;
                    saddr_size = sizeof(saddr);

                    data_size = recvfrom(sock_row, buffer, 65536, 0 , &saddr, &saddr_size);
                    if(data_size < 0 )
                    {
                        errMsg("recv error, failed to recive packets");
                    }
                    processPacket(buffer, data_size);
                }
                close(sock_row);
                fprintf(stderr , "Finished.\n");
}
