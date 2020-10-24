int write_custom_line(int sockfd, const char *line)
{
	size_t len = strlen(line) + EXTRA_CUSTOM_LEN;
	size_t bytes_written;
	char *customline;
	
	if((customline = mymalloc(len)) == 0)
		return errno;
	
	snprintf(customline, len, "%s\r\n", line);
	len = strlen(customline);
#if 0
	fprintf(stderr, "write_custom_line = [%s]\n", customline);
	fprintf(stderr, "strlen(customline) = [%d]\n", strlen(customline));
#endif
	bytes_written = writen(sockfd, customline, len);
	if(bytes_written < 0) {
		log_error(LOG_ARGS, "Could not write customline");
		myfree(customline);
		return errno;
	}
	myfree(customline);
	return 0;
}