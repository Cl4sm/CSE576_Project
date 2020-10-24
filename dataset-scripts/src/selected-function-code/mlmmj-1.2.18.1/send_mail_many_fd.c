int send_mail_many_fd(int sockfd, const char *from, const char *replyto,
		      char *mailmap, size_t mailsize, int subfd,
		      const char *listaddr, const char *listdelim,
		      const char *archivefilename, const char *listdir,
		      const char *mlmmjbounce, const char *hdrs, size_t hdrslen,
		      const char *body, size_t bodylen)
{
	int res, ret, i;
	struct strlist stl;

	stl.strs = NULL;
	stl.count = 0;

	do {
		res = getaddrsfromfd(&stl, subfd, maxverprecips);
		if(stl.count == maxverprecips) {
			ret = send_mail_many_list(sockfd, from, replyto,
					mailmap, mailsize, &stl, listaddr,
					listdelim, archivefilename, listdir,
					mlmmjbounce, hdrs, hdrslen,
					body, bodylen);
			for(i = 0; i < stl.count; i++)
				myfree(stl.strs[i]);
			if(ret < 0)
				return ret;
			stl.count = 0;
		}
	} while(res > 0);

	if(stl.count) {
		ret = send_mail_many_list(sockfd, from, replyto, mailmap,
				mailsize, &stl, listaddr, listdelim,
				archivefilename, listdir, mlmmjbounce,
				hdrs, hdrslen, body, bodylen);
		for(i = 0; i < stl.count; i++)
			myfree(stl.strs[i]);
		stl.count = 0;
		return ret;
	}

	return 0;
}