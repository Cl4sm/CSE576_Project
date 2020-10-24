prodpvmd()
{
	static char dummy[TDFRAGHDR];

	if (pvmdsock == -1) {
		if ((pvmdsock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			pvmlogperror("prodpvmd() socket");
			return;
		}
		if (connect(pvmdsock, (struct sockaddr*)&pvmdsad, sizeof(pvmdsad))
		== -1) {
			pvmlogperror("prodpvmd() connect");
			close(pvmdsock);
			pvmdsock = -1;
			return;
		}
		BZERO(dummy, sizeof(dummy));

	} else {
		write(pvmdsock, dummy, TDFRAGHDR);
	}
}
