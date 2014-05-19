#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MYPORT  "3490"
#define BACKLOG	10

int main(int argc, char *argv[])
{
	struct sockaddr_storage customer_addr;
	int	addr_size;
	struct addrinfo hints, *res;

	int sockfd, new_fd;
	int n;

	char mesg[1024];
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	getaddrinfo(NULL, MYPORT, &hints, &res);

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {
		printf("Failed to setsockopt\n");
		return -1;
	}
	
	if(sockfd < 0) {
		printf("Failed to socket\n");
	}

	if (bind(sockfd, res->ai_addr, res->ai_addrlen) !=0) {
		printf("Failed to bind\n");
	}

	if(listen(sockfd, BACKLOG) < 0 ) {
		printf("Failed to listen\n");
	}

	addr_size = sizeof(customer_addr);
	new_fd = accept(sockfd, (struct sockaddr *)&customer_addr, (socklen_t *)&addr_size);
	if(new_fd < 0){
		printf("Failed accept\n");
	}

	struct timeval tTimeout = {1, 0};

	for(;;) {
		memset(mesg, 0, sizeof(mesg));
		fd_set seFdSet;
		FD_ZERO(&seFdSet);
		FD_SET(sockfd, &seFdSet);
		if(select(sockfd + 1, &seFdSet, NULL, NULL, &tTimeout) < 0){
			fprintf(stderr, "Error\n");
		}

		n = recvfrom(new_fd, mesg, sizeof(mesg), MSG_DONTWAIT, (struct sockaddr *)&customer_addr, (socklen_t *)&addr_size);
		//n = recvfrom(new_fd, mesg, sizeof(mesg), 0, (struct sockaddr *)&customer_addr, (socklen_t *)&addr_size);
		if(strncmp("Bye", mesg, 3) == 0){
			break;
			close(new_fd);
		}
		printf("%s", mesg);
		
	}
	close(sockfd);

/*
	struct sockaddr_in servaddr, cliaddr;
	socklen_t	clilen;
	char mesg[1000];

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(3200);

	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("connect");
		exit(1);
	}

	listen(listenfd, 3);

	for (;;) {
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);


		for (;;) {
			n = recvfrom(connfd, mesg, sizeof(mesg), 0, (struct sockaddr *)&cliaddr, &clilen);
			char myMesg[n];
			char *sep = ",";
			char *pMesg = myMesg;
			char *strTmp;
			strncpy(myMesg, mesg, n);

			strTmp = strtok(pMesg, sep);
			while (strTmp != NULL) {
				printf("%s\n", strTmp);
				strTmp = strtok(NULL, sep);
			};
			sleep (1);

		}

		close(listenfd);
	}
		close(connfd);

*/
}
