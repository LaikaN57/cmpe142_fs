// man 7 unix
// man 2 bind

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>

#define MY_SOCK_PATH	"/tmp/sjfs"
#define LISTEN_BACKLOG	50

// TODO: SIGPIPE handler for socket?

int main(int argc, char *argv[]) {
	int sfd;
	int cfd;
	struct sockaddr_un my_addr;
	struct sockaddr_un peer_addr;
	socklen_t peer_addr_size;

	// create socket
	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sfd == -1) {
		perror("socket");
		exit(EXIT_FAILURE); // we cannot recover from this
	}

	// bind socket to pathname
	memset(&my_addr, 0, sizeof(struct sockaddr_un));
	my_addr.sun_family = AF_UNIX;
	strncpy(my_addr.sun_path, MY_SOCK_PATH, sizeof(my_addr.sun_path) - 1);
	if(bind(sfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_un)) == -1) {
		perror("bind");
		exit(EXIT_FAILURE); // we cannot recover from this
	}

	// start listening for socket connections
	if(listen(sfd, LISTEN_BACKLOG) == -1) {
		perror("listen");
		exit(EXIT_FAILURE); // we cannot recover from this
	}

	// ACCEPT LOOP
	while(1) { // TODO: allow some kind of exit condition
		// accept a new connection
		memset(&peer_addr, 0, sizeof(struct sockaddr_un));
		peer_addr_size = sizeof(struct sockaddr_un);
		cfd = accept(sfd, (struct sockaddr *) &peer_addr, &peer_addr_size); // TODO: make this non-blocking for user input from console?
		if(cfd == -1) {
			perror("accept");
			continue; // do we need to do any other cleanup?
		}

		// RECV LOOP
		while(1) { // TODO: allow some kind of exit condition
			// TODO: make a spec for passing messages through this socket
		} // RECV LOOP END

		if(close(cfd) == -1) {
			perror("c close");
			continue;
		}
	} // ACCEPT LOOP END

	if(close(sfd) == -1) {
		perror("s close");
	}

	if(unlink(MY_SOCK_PATH) == -1) {
		perror("unlink");
	}

	return 0;
}


