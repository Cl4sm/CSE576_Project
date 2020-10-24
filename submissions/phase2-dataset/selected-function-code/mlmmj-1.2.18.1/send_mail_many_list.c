int send_mail_many_list(int sockfd, const char *from, const char *replyto,
		   char *mailmap, size_t mailsize, struct strlist *addrs,
		   const char *listaddr, const char *listdelim,
		   const char *archivefilename, const char *listdir,
		   const char *mlmmjbounce, const char *hdrs, size_t hdrslen,
		   const char *body, size_t bodylen)
{
	int res = 0, i, status;
	char *bounceaddr, *addr, *index;

	for(i = 0; i < addrs->count; i++) {
		addr = addrs->strs[i];
		if(strchr(addr, '@') == NULL) {
			errno = 0;
			log_error(LOG_ARGS, "No @ in address, ignoring %s",
					addr);
			continue;
		}
		if(gotsigterm && listaddr && archivefilename) {
			/* we got SIGTERM, so save the addresses and bail */
			log_error(LOG_ARGS, "TERM signal received, "
						"shutting down.");
			index = get_index_from_filename(archivefilename);
			status = requeuemail(listdir, index, addrs, i);
			myfree(index);
			return status;
		}
		if(from) {
			res = send_mail(sockfd, from, addr, replyto,
					    mailmap, mailsize, listdir, NULL,
					    hdrs, hdrslen, body, bodylen);
		} else {
			bounceaddr = bounce_from_adr(addr, listaddr, listdelim,
						     archivefilename, listdir);
			res = send_mail(sockfd, bounceaddr, addr, replyto,
				  mailmap, mailsize, listdir, mlmmjbounce,
				  hdrs, hdrslen, body, bodylen);
			myfree(bounceaddr);
		}
		if(res && listaddr && archivefilename) {
			/* we failed, so save the addresses and bail */
			index = get_index_from_filename(archivefilename);
			status = requeuemail(listdir, index, addrs, i);
			myfree(index);
			return status;
		}
	}
	return 0;
}	