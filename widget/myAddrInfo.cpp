#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <ifaddrs.h>

int main(int argc, char *argv[])
{
	struct ifaddrs *myAddr, *ifa;
	void *in_addr;
	char buf[64];

	if(getifaddrs(&myAddr) != 0) {
		fprintf(stderr, "Error\n");
	}

	for (ifa = myAddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		if(ifa->ifa_addr == NULL)
			continue;
		if (!(ifa->ifa_flags & IFF_UP))
			continue;

		switch (ifa->ifa_addr->sa_family)
		{
			case AF_INET:
			{
				struct sockaddr_in *s4 = (struct sockaddr_in *)ifa->ifa_addr;
				in_addr = &s4->sin_addr;
				break;
			}
			case AF_INET6:
			{
				struct sockaddr_in6 *s6 = (struct sockaddr_in6 *)ifa->ifa_addr;
				in_addr = &s6->sin6_addr;
				break;
			}
			default:
				continue;
		}
	}

	if (!inet_ntop(ifa->ifa_addr->sa_family, in_addr, buf, sizeof(buf))) {
		fprintf(stderr, "Error\n");
	}
	else {
		printf("%s: %s\n", ifa->ifa_name, buf);
	}

	return 0;
}
