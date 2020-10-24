int run_digests(const char *listdir, const char *mlmmjsend)
{
	char *lasttimestr, *lastindexstr, *lastissuestr;
	char *digestname, *indexname;
	char *digestintervalstr, *digestmaxmailsstr;
	char *s1, *s2, *s3;
	time_t digestinterval, t, lasttime;
	long digestmaxmails, lastindex, index, lastissue;
	int fd, indexfd, lock;
	size_t lenbuf, lenstr;

	if (statctrl(listdir, "noarchive")) {
		return 0;
	}
	
	digestintervalstr = ctrlvalue(listdir, "digestinterval");
	if (digestintervalstr) {
		digestinterval = (time_t)atol(digestintervalstr);
		myfree(digestintervalstr);
	} else {
		digestinterval = (time_t)DIGESTINTERVAL;
	}

	digestmaxmailsstr = ctrlvalue(listdir, "digestmaxmails");
	if (digestmaxmailsstr) {
		digestmaxmails = atol(digestmaxmailsstr);
		myfree(digestmaxmailsstr);
	} else {
		digestmaxmails = DIGESTMAXMAILS;
	}

	digestname = concatstr(2, listdir, "/lastdigest");
	fd = open(digestname, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		log_error(LOG_ARGS, "Could not open '%s'", digestname);
		myfree(digestname);
		return 1;
	}
	
	lock = myexcllock(fd);
	if(lock) {
		log_error(LOG_ARGS, "Error locking lastdigest");
		myfree(digestname);
		close(fd);
		return 1;
	}

	s1 = mygetline(fd);

	/* Syntax is lastindex:lasttime or lastindex:lasttime:lastissue */
	if (s1 && (lasttimestr = strchr(s1, ':'))) {
		*(lasttimestr++) = '\0';
		if ((lastissuestr = strchr(lasttimestr, ':'))) {
			*(lastissuestr++) = '\0';
			lastissue = atol(lastissuestr);
		} else {
			lastissue = 0;
		}
		lasttime = atol(lasttimestr);
		lastindexstr = s1;
		lastindex = atol(lastindexstr);
	} else {
		if (s1 && (strlen(s1) > 0)) {
			log_error(LOG_ARGS, "'%s' contains malformed data",
					digestname);
			myfree(digestname);
			myfree(s1);
			close(fd);
			return 1;
		}
		/* If lastdigest is empty, we start from scratch */
		lasttime = 0;
		lastindex = 0;
		lastissue = 0;
	}
	
	indexname = concatstr(2, listdir, "/index");
	indexfd = open(indexname, O_RDONLY);
	if (indexfd < 0) {
		log_error(LOG_ARGS, "Could not open '%s'", indexname);
		myfree(digestname);
		myfree(indexname);
		myfree(s1);
		close(fd);
		return 1;
	}
	s2 = mygetline(indexfd);
	close(indexfd);
	if (!s2) {
		/* If we don't have an index, no mails have been sent to the
		 * list, and therefore we don't need to send a digest */
		myfree(digestname);
		myfree(indexname);
		myfree(s1);
		close(fd);
		return 1;
	}

	index = atol(s2);
	t = time(NULL);

	if ((t - lasttime >= digestinterval) ||
			(index - lastindex >= digestmaxmails)) {

		if (index > lastindex+digestmaxmails)
			index = lastindex+digestmaxmails;

		if (index > lastindex) {
			lastissue++;
			send_digest(listdir, lastindex+1, index, lastissue, NULL, mlmmjsend);
		}

		if (lseek(fd, 0, SEEK_SET) < 0) {
			log_error(LOG_ARGS, "Could not seek '%s'", digestname);
		} else {
			/* index + ':' + time + ':' + issue + '\n' + '\0' */
			lenbuf = 20 + 1 + 20 + 1 + 20 + 2;
			s3 = mymalloc(lenbuf);
			lenstr = snprintf(s3, lenbuf, "%ld:%ld:%ld\n", index, (long)t, lastissue);
			if (lenstr >= lenbuf)
				lenstr = lenbuf - 1;
			if (writen(fd, s3, lenstr) == -1) {
				log_error(LOG_ARGS, "Could not write new '%s'",
						digestname);
			}
			myfree(s3);
		}
	}

	myfree(digestname);
	myfree(indexname);
	myfree(s1);
	myfree(s2);
	close(fd);
	
	return 0;
}