int write_replyto(int sockfd, const char *replyaddr)
{
	size_t len = (size_t)(strlen(replyaddr) + EXTRA_REPLYTO_LEN);
	char *replyto;
	size_t bytes_written;
	
	if((replyto = mymalloc(len)) == 0)
		return errno;

	snprintf(replyto, len, "Reply-To: %s\r\n", replyaddr);
	len = strlen(replyto);

#if 0
	fprintf(stderr, "\nwrite_replyto, replyto = [%s]\n", replyto);
#endif
	bytes_written = writen(sockfd, replyto, len);
	if(bytes_written < 0) {
		log_error(LOG_ARGS, "Could not write Reply-To header");
		myfree(replyto);
		return errno;
	}
	myfree(replyto);
	return 0;
}