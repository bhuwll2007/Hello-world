#include "../LTEVUtil/LTE_V2X_RAW.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <linux/sockios.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>


int g_rawsock;

struct iphdr g_iph;
struct udphdr g_udph;
struct dsmp_msg_t g_dsmph;

int Raw_init(const char *interface) {
	int s;
	int ret;
	struct sockaddr_ll addr;
	struct ifreq ifr;
	int optval = 1;

	s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
	if (s < 0)
		printf("socket ret %d, %s\n", s, strerror(errno));

	memset(&addr, 0, sizeof(addr));

	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, interface);
	ioctl(s, SIOCGIFINDEX, &ifr);

	addr.sll_family = PF_PACKET;
	addr.sll_ifindex = ifr.ifr_ifindex;

	ret = bind(s, (struct sockaddr *) &addr, sizeof(addr));
	if (ret < 0)
	{
		printf("bind %s ret %d, %s\n", interface, ret, strerror(errno));
	}
	optval = 1;
	ret = setsockopt(s, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));
	if (ret < 0)
		printf("setsocket2 ret %d, %s\n", ret, strerror(errno));
	g_rawsock = s;

	//init global variable
	g_iph.version = 4;
	g_iph.ihl = 5;    // ��ͷ���� 5*32 = 160bit = 20B
	g_iph.tos = 0;    // ��������
	g_iph.tot_len = 0;

	g_iph.ttl = 64; // hops��������
	g_iph.protocol = 17; // UDP
	g_iph.check = 0;
	g_iph.saddr = inet_addr("192.168.40.199");
	g_iph.daddr = inet_addr("192.168.40.255");

	g_udph.source = htons(6666);
	g_udph.dest = htons(6666);
	g_udph.len = htons(0);
	g_udph.check = 0;

	g_dsmph.ueid = 0;
	g_dsmph.psid = 0;

	g_dsmph.power = 10;
	g_dsmph.priority = 0;
	g_dsmph.period = 0;
	g_dsmph.proc_id = 0;

	g_dsmph.plen = 0;
	g_dsmph.test_mode = 0;
	g_dsmph.test_sfn = 0;

	return s;
}

int Raw_release() {
	int fd = g_rawsock;
	close(fd);
	return 0;
}

unsigned char rx_buf[4096] = { 0 };
unsigned char tx_buf[4096] = { 0 };

int RawSendData(int nAID, char buf[], int buflen) {
	struct sockaddr_ll addr;
	int ret;
	struct iphdr *iph = NULL;
	struct udphdr *udph = NULL;
	struct dsmp_msg_t *dsmph = NULL;
	char *payload = NULL;
	socklen_t slen = 0;
	int plen = buflen;
	struct ifreq ifr;

	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, "lmi40");
	ioctl(g_rawsock, SIOCGIFINDEX, &ifr);

	memset(&addr, 0, sizeof(addr));
	addr.sll_family = PF_PACKET;
	addr.sll_ifindex = ifr.ifr_ifindex;
	addr.sll_halen = 0;
	addr.sll_hatype = ARPHRD_NONE;
	/* lmi40 has no ether head, so directly using iphdr */
	iph = (struct iphdr *) tx_buf;
	udph = (struct udphdr *) (tx_buf + sizeof(struct iphdr));
	dsmph = (struct dsmp_msg_t *) (tx_buf + sizeof(struct iphdr)
			+ sizeof(struct udphdr));
	payload = (char *) (tx_buf + sizeof(struct iphdr) + sizeof(struct udphdr)
			+ sizeof(struct dsmp_msg_t));

	memcpy(iph, &g_iph, sizeof(struct iphdr));
	memcpy(udph, &g_udph, sizeof(struct udphdr));
	memcpy(dsmph, &g_dsmph, sizeof(struct dsmp_msg_t));

	iph->tot_len = ((sizeof(struct iphdr) + sizeof(struct udphdr)
			+ sizeof(struct dsmp_msg_t) + plen));

	udph->dest = htons(nAID);
	udph->len = htons(sizeof(struct udphdr) + sizeof(struct dsmp_msg_t) + plen);
	udph->check = 0;

	dsmph->plen = plen;

	//拷贝数据
	memcpy(payload, buf, buflen);

	slen = sizeof(struct sockaddr_ll);
	ret = sendto(g_rawsock, tx_buf,
			sizeof(struct iphdr) + sizeof(struct udphdr)
					+ sizeof(struct dsmp_msg_t) + plen, 0,
			(struct sockaddr *) &addr, slen);
	if (ret <= 0) {
		printf("sendto %d failed, %s\n", ret, strerror(errno));
	}

	return ret;
}
int RawRecvData(char buf[]) {

	struct sockaddr_ll addr;
	int ret = 0;
	struct iphdr *iph = NULL;
	struct udphdr *udph = NULL;
	struct dsmp_msg_t *dsmph = NULL;
	socklen_t slen = 0;

	ret = recvfrom(g_rawsock, rx_buf, sizeof(rx_buf), 0,
			(struct sockaddr *) &addr, &slen);
	if (ret > 0) {
//		printf("hardware type %u\n", addr.sll_hatype);
//		printf("packet type %u\n", addr.sll_pkttype);
//		printf("hardware addr len %u\n", addr.sll_halen);
//		printf("recv %dB, %s\n", ret, strerror(errno));
		/* | IP header | UDP header | catt header | user payload |*/
		iph = (struct iphdr *) (rx_buf);
		udph = (struct udphdr *) (rx_buf + sizeof(*iph));
		dsmph = (struct dsmp_msg_t *) (rx_buf + sizeof(*iph) + sizeof(*udph));
		/* AID: udp dsp port */
//		printf("payload %uB, AID %u\n", dsmph->plen, ntohs(udph->dest));
		ret = dsmph->plen;
		if (ret > 2000) {
			return 0;
		}
		memcpy(buf, (rx_buf + sizeof(*iph) + sizeof(*udph) + sizeof(*dsmph)),
				dsmph->plen);
	} else {
//		printf("recv %d failed, %s\n", ret, strerror(errno));
	}

	return ret;
}
