int write_mail_from(int sockfd, const char *from_addr, const char *extra)
{
	size_t len = (size_t)(strlen(from_addr) + EXTRA_FROM_LEN +
			strlen(extra) + 2);
	char *mail_from;
	size_t bytes_written;

	mail_from = mymalloc(len);

	if(extra && extra[0] != '\0') {
		if(extra[0] == ' ') extra++;
		snprintf(mail_from, len, "MAIL FROM: <%s> %s\r\n", from_addr,
				extra);
	} else
		snprintf(mail_from, len, "MAIL FROM: <%s>\r\n", from_addr);

	len = strlen(mail_from);

#if 0
	fprintf(stderr, "%s", mail_from);
#endif
	bytes_written = writen(sockfd, mail_from, len);
	if(bytes_written < 0) {
		log_error(LOG_ARGS, "Could not write FROM");
		myfree(mail_from);
		return errno;
	}
	myfree(mail_from);
	return 0;
}