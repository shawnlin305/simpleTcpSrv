#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[])
{
	struct addrinfo hints;
	struct addrinfo *result, *p;
	int status;
	char ipstr[INET6_ADDRSTRLEN];

	if (argc != 2) {
		fprintf(stderr, "usage : showip hostname");
		return -1;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if((status = getaddrinfo(argv[1], "80", &hints, &result)) != 0){
		fprintf(stderr, "getaddrinfo error : %d", status);
	}

	printf("IP Address of %s:\n\n", argv[1]);

	for(p = result; p != NULL; p = p->ai_next) {
		void *addr;
		char *ipver;
		int port;

		if( p->ai_family == AF_INET) {
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		}
		else {
			struct sockaddr_in6 *ipv6 =  (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}

		inet_ntop(p->ai_family, addr, ipstr, INET6_ADDRSTRLEN);
		printf(" %s : %s:%d\n", ipver, ipstr, ntohs(((struct sockaddr_in *)p)->sin_port));
	}

//	getaddrinfo()
//	inet_pton
	return 0;
}
