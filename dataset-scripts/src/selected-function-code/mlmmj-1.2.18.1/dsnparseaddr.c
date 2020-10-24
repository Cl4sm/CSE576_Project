char *dsnparseaddr(const char *mailname)
{
	int fd, indsn = 0, i;
	char *line, *linedup, *search, *addr = NULL;
	struct email_container emails = { 0, NULL };

	fd = open(mailname, O_RDONLY);
	if(fd < 0) {
		log_error(LOG_ARGS, "Could not open bounceindexfile %s",
				mailname);
		return NULL;
	}

	while((line = gethdrline(fd))) {
		linedup = mystrdup(line);
		for(i = 0; line[i]; i++)
			linedup[i] = tolower(line[i]);
		search = strstr(linedup, "message/delivery-status");
		myfree(linedup);
		if(search)
			indsn = 1;
		if(indsn) {
			i = strncasecmp(line, "Final-Recipient:", 16);
			if(i == 0) {
				find_email_adr(line, &emails);
				if(emails.emailcount > 0) {
					addr = mystrdup(emails.emaillist[0]);
					for(i = 0; i < emails.emailcount; i++)
						myfree(emails.emaillist[i]);
					myfree(emails.emaillist);
				} else {
					addr = NULL;
				}
				myfree(line);
				return addr;
			}
		}
		myfree(line);
	}

	return NULL;
}