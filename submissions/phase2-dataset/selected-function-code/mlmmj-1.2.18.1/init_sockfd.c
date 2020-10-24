void init_sockfd(int *sockfd, const char *relayhost, unsigned short port)
{
	int len, on;
	struct sockaddr_in addr;

	if (getenv("MLMMJ_TESTING")) {
		relayhost = "127.0.0.1";
		port = 10025;
	}

	*sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if(*sockfd == -1) {
		log_error(LOG_ARGS, "Could not get socket");
		return;
	}
	addr.sin_family = PF_INET;
	addr.sin_addr.s_addr = inet_addr(relayhost);
	addr.sin_port = htons(port);
	len = sizeof(addr);
	if(connect(*sockfd, (struct sockaddr *)&addr, len) == -1) {
		log_error(LOG_ARGS, "Could not connect to %s", relayhost);
		close(*sockfd);
		*sockfd = -1;
		return;
	}

	on = 1;
	if(setsockopt(*sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&on,
				sizeof(on)) < 0)
		log_error(LOG_ARGS, "Could not set SO_KEEPALIVE");
}