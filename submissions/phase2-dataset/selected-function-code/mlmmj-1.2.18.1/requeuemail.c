int requeuemail(const char *listdir, const char *index, struct strlist *addrs,
		int addrcount)
{
	int addrfd, i;
	char *dirname, *addrfilename, *addr;
	
	dirname = concatstr(3, listdir, "/requeue/", index);
	if(mkdir(dirname, 0750) < 0 && errno != EEXIST) {
		log_error(LOG_ARGS, "Could not mkdir(%s) for "
				"requeueing. Mail cannot "
				"be requeued.", dirname);
		myfree(dirname);
		return -1;
	}
	addrfilename = concatstr(2, dirname, "/subscribers");
	myfree(dirname);
	addrfd = open(addrfilename, O_WRONLY|O_CREAT|O_APPEND,
			S_IRUSR|S_IWUSR);
	if(addrfd < 0) {
		log_error(LOG_ARGS, "Could not open %s",
				addrfilename);
		myfree(addrfilename);
		return -1;
	} else {
		/* Dump the remaining addresses. We dump the remaining before
		 * we write the failing address to ensure the potential good
		 * ones will be tried first when mlmmj-maintd sends out mails
		 * that have been requeued. addrcount was so far we were */
		for(i = addrcount + 1; i < addrs->count; i++) {
			addr = concatstr(2, addrs->strs[i], "\n");
			if(writen(addrfd, addr, strlen(addr)) < 0) {
				log_error(LOG_ARGS, "Could not add [%s] "
						    "to requeue file", addr);
				return -1;
			}
			myfree(addr);
		}
		addr = concatstr(2, addrs->strs[addrcount], "\n");
		if(writen(addrfd, addr, strlen(addr)) < 0) {
			log_error(LOG_ARGS, "Could not add [%s] to requeue "
					"file", addr);
			return -1;
		}
		myfree(addr);
	}
	myfree(addrfilename);
	close(addrfd);

	return 0;
}