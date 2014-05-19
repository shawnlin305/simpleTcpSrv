#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
	struct sockaddr_in sa;

	inet_pton(AF_INET, "192.168.0.20", &sa.sin_addr);
	printf("%d\n", sa.sin_addr);
	printf("%s\n", inet_ntoa(sa.sin_addr));
	char ip4[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);
	printf("ip4 : %s\n", ip4);


	return 0;
}
