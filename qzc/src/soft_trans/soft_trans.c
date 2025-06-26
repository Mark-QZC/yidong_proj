#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libnetfilter_queue/libnetfilter_queue.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>

#ifndef NF_DROP
#define NF_DROP 2
#endif

#ifndef NF_STOLEN
#define NF_STOLEN 4
#endif

static int cb(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfa, void *data) {
    struct nfqnl_msg_packet_hdr *ph;
    unsigned char *pkt;
    int ret;

    ph = nfq_get_msg_packet_hdr(nfa);
    if (!ph) {
        return NF_DROP;
    }

    ret = nfq_get_payload(nfa, &pkt);
    if (ret <= 0) {
        return NF_DROP;
    }

    struct iphdr *ip = (struct iphdr *)pkt;
    printf("Packet received: src=%s, dst=%s\n",
           inet_ntoa(*(struct in_addr *)&ip->saddr),
           inet_ntoa(*(struct in_addr *)&ip->daddr));

    ip->daddr = inet_addr("192.168.1.100");

    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock < 0) {
        perror("socket");
        return NF_DROP;
    }

    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = ip->daddr;

    if (sendto(sock, pkt, ret, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("sendto");
        close(sock);
        return NF_DROP;
    }

    close(sock);

    return NF_DROP;
}

int main(int argc, char **argv) {
    struct nfq_handle *h;
    struct nfq_q_handle *qh;
    struct nfnl_handle *nh;
    int fd;
    int rv;

    printf("Starting soft forwarding interface...\n");

    h = nfq_open();
    if (!h) {
        fprintf(stderr, "Error during nfq_open()\n");
        exit(EXIT_FAILURE);
    }

    if (nfq_bind_pf(h, AF_INET) < 0) {
        fprintf(stderr, "Error during nfq_bind_pf()\n");
        exit(EXIT_FAILURE);
    }

    qh = nfq_create_queue(h, 1, &cb, NULL);
    if (!qh) {
        fprintf(stderr, "Error during nfq_create_queue()\n");
        exit(EXIT_FAILURE);
    }

    if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
        fprintf(stderr, "Error during nfq_set_mode()\n");
        exit(EXIT_FAILURE);
    }

    fd = nfq_fd(h);

    char buf[4096];
    while ((rv = recv(fd, buf, sizeof(buf), 0)) >= 0) {
        nfq_handle_packet(h, buf, rv);
    }

    printf("Unbinding from queue 1\n");
    nfq_destroy_queue(qh);

    printf("Closing library handle\n");
    nfq_close(h);

    exit(EXIT_SUCCESS);
}