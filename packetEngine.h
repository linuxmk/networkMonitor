#ifndef PACKETENGINE_H
#define PACKETENGINE_H

#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netdb.h>

void initSniffer(void);
void initSnifferRaw(void);
void process_packet(u_char *handle, const struct pcap_pkthdr *pkthdr, const u_char *packet);
void processPacket(u_char * packet, int size); // for raw packets
void print_tcp_packet(const u_char * packet, int size);
void print_ethernet_header(const u_char *packet, int size);
void print_ip_header(const u_char *packet, int size);
void PrintData(const u_char * packet, int size);
void print_udp_packet(const u_char *Buffer , int Size);
char * getSrcIp(const u_char *packet, char *sIp);
char * getDestIp(const u_char *packet, char *dIp);
char *getProtocol(const u_char *packet, char *protocol);
int getSrcPort(const u_char *packet);
int getDestPort(const u_char *packet);



#endif // PKTINIT_H
