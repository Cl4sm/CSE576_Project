int write_rcpt_to(int sockfd, const char *rcpt_addr)
{
	size_t len = (size_t)(strlen(rcpt_addr) + EXTRA_RCPT_LEN);
	char *rcpt_to;
	size_t bytes_written;
	
	if((rcpt_to = mymalloc(len)) == 0)
		return errno;

	snprintf(rcpt_to, len, "RCPT TO: <%s>\r\n", rcpt_addr);
	len = strlen(rcpt_to);
#if 0
	log_error(LOG_ARGS, "%s", rcpt_to);
#endif
	bytes_written = writen(sockfd, rcpt_to, len);
	if(bytes_written < 0) {
		log_error(LOG_ARGS, "Could not write TO");
		myfree(rcpt_to);
		return errno;
	}
	myfree(rcpt_to);
	return 0;
}