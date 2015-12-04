#include <asm/types.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>

#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include <arpa/inet.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <getopt.h>

#include <linux/connector.h>

#define DEBUG
#define NETLINK_CONNECTOR 	11

#define CN_TEST_IDX		CN_NETLINK_USERS + 3
#define CN_TEST_VAL		0x456

#ifdef DEBUG
#define ulog(f, a...) printf(f, ##a)
#else
#define ulog(f, a...) do {} while (0)
#endif

static int need_exit;
static __u32 seq;

static int netlink_send(int s, struct cn_msg *msg) {
	struct nlmsghdr *nlh;
	unsigned int size;
	int err;
	char buf[128];
	struct cn_msg *m;

	size = NLMSG_SPACE(sizeof(struct cn_msg) + msg->len);

	nlh = (struct nlmsghdr *)buf;
	nlh->nlmsg_seq = seq++;
	nlh->nlmsg_pid = getpid();
	nlh->nlmsg_type = NLMSG_DONE;
	nlh->nlmsg_len = size;
	nlh->nlmsg_flags = 0;

	m = NLMSG_DATA(nlh);
	memcpy(m, msg, sizeof(*m) + msg->len);

	err = send(s, nlh, size, 0);
	if (err == -1)
		ulog("Failed to send: %s [%d].\n",
			strerror(errno), errno);

	return err;
}

void do_work(unsigned char *buffer) {
	sjfs_request_packet_t * packet;
	FILE * fd;

	packet = (sjfs_request_packet_t *) buffer;

	// open file numbered packet->inode
	fd = fopen("/dev/" + packet->inode, "wb");
	if(!fd) {
		return;
	}

	// seek file to pos
	if(!fseek(fd, packet->count, SEEK_SET)) {
		return;
	}

	switch(packet->opcode) {
		case SJFS_OPCODE_READ:
			// send back a response packet
			// malloc a data buffer of size packet->count
			// fread count bytes
			// send data buffer in response packet

/*
        if (send_msgs) {
                int i, j;

                memset(buf, 0, sizeof(buf));

                data = (struct cn_msg *)buf;

                data->id.idx = CN_TEST_IDX;
                data->id.val = CN_TEST_VAL;
                data->seq = seq++;
                data->ack = 0;
                data->len = 0;

                len = netlink_send(s, data);

                return 0;
        }
*/


			break;
		case SJFS_OPCODE_WRITE:
			// write data to file
			fwrite((const void *) packet->data, sizeof(unsigned char), packet->count, fd);
			break;
		default:
			fclose(fd);
			return;
	}

	fclose(fd);
	return;
}

int main(int argc, char *argv[]) {
	int s;
	char buf[65536];
	int len;
	struct nlmsghdr *reply;
	struct sockaddr_nl l_local;
	struct cn_msg *data;
	struct pollfd pfd;

	memset(buf, 0, sizeof(buf));

	s = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
	if (s == -1) {
		perror("socket");
		return -1;
	}

	l_local.nl_family = AF_NETLINK;
	l_local.nl_groups = -1; /* bitmask of requested groups */
	l_local.nl_pid = 0;

	ulog("subscribing to %u.%u\n", CN_TEST_IDX, CN_TEST_VAL);

	if (bind(s, (struct sockaddr *)&l_local, sizeof(struct sockaddr_nl)) == -1) {
		perror("bind");
		close(s);
		return -1;
	}

	pfd.fd = s;

	while (!need_exit) {
		pfd.events = POLLIN;
		pfd.revents = 0;
		switch (poll(&pfd, 1, -1)) {
			case 0:
				need_exit = 1;
				break;
			case -1:
				if (errno != EINTR) {
					need_exit = 1;
					break;
				}
				continue;
		}
		if (need_exit)
			break;

		memset(buf, 0, sizeof(buf));
		len = recv(s, buf, sizeof(buf), 0);
		if (len == -1) {
			perror("recv buf");
			close(s);
			return -1;
		}
		reply = (struct nlmsghdr *)buf;

		switch (reply->nlmsg_type) {
		case NLMSG_ERROR:
			printf("Error message received.\n");
			break;
		case NLMSG_DONE:
			data = (struct cn_msg *)NLMSG_DATA(reply);

			printf("[%x.%x] [%08u.%08u].\n",
				data->id.idx, data->id.val, data->seq, data->ack);
			break;

			do_work(data->data);
		default:
			break;
		}
	}

	close(s);
	return 0;
}
