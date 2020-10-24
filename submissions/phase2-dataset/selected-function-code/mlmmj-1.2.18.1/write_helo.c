int write_helo(int sockfd, const char *hostname)
{
	size_t len = (size_t)(strlen(hostname) + EXTRA_HELO_LEN);
	char *helo;
	size_t bytes_written;
	
	if((helo = mymalloc(len)) == 0)
		return errno;
	snprintf(helo, len, "HELO %s\r\n", hostname);
	len = strlen(helo);
#if 0
	fprintf(stderr, "\nwrite_helo, helo = [%s]\n", helo);
#endif
	bytes_written = writen(sockfd, helo, len);
	if(bytes_written < 0) {
		log_error(LOG_ARGS, "Could not write HELO");
		myfree(helo);
		return errno;
	}
	myfree(helo);
	return 0;
}