int send_digest(const char *listdir, int firstindex, int lastindex,
		int issue, const char *addr, const char *mlmmjsend)
{
	int i, fd, archivefd, status, hdrfd;
	size_t len;
	text * txt;
	char buf[100];
	char *tmp, *queuename = NULL, *archivename, *subject = NULL, *line = NULL;
	char *boundary, *listaddr, *listdelim, *listname, *listfqdn;
	pid_t childpid, pid;
	thread_list_state * tls;

	if (addr) {
		errno = 0;
		log_error(LOG_ARGS, "send_digest() does not support sending "
				"digest mails to only one recipient yet");
		return -1;
	}

	if (firstindex > lastindex)
		return -1;
	
	do {
		tmp = random_str();
		myfree(queuename);
		queuename = concatstr(3, listdir, "/queue/", tmp);
		myfree(tmp);
		fd = open(queuename, O_RDWR|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR);
	} while ((fd < 0) && (errno == EEXIST));

	if (fd < 0) {
		log_error(LOG_ARGS, "Could not open digest queue file '%s'",
				queuename);
		myfree(queuename);
		return -1;
	}

	tmp = concatstr(2, listdir, "/control/customheaders");
	hdrfd = open(tmp, O_RDONLY);
	myfree(tmp);

	boundary = random_str();

	listaddr = getlistaddr(listdir);
	listname = genlistname(listaddr);
	listfqdn = genlistfqdn(listaddr);
	listdelim = getlistdelim(listdir);
	
	txt = open_text_file(listdir, "digest");
	if (txt == NULL) {
		log_error(LOG_ARGS, "Could not open listtext 'digest'");
		goto fallback_subject;
	}

	snprintf(buf, sizeof(buf), "%d", firstindex);
	register_unformatted(txt, "digestfirst", buf);

	snprintf(buf, sizeof(buf), "%d", lastindex);
	register_unformatted(txt, "digestlast", buf);

	if (lastindex == firstindex) {
		snprintf(buf, sizeof(buf), "%d", firstindex);
	} else {
		snprintf(buf, sizeof(buf), "%d-%d", firstindex, lastindex);
	}
	register_unformatted(txt, "digestinterval", buf);

	snprintf(buf, sizeof(buf), "%d", issue);
	register_unformatted(txt, "digestissue", buf);

	register_unformatted(txt, "digestthreads", "%digestthreads%"); /* DEPRECATED */

	tls = init_thread_list(listdir, firstindex, lastindex);
	register_formatted(txt, "digestthreads", rewind_thread_list,
			get_thread_list_line, tls);

	line = get_processed_text_line(txt, 1, listaddr, listdelim, listdir);

	if (line == NULL) {
		log_error(LOG_ARGS, "No content in digest listtext");
		goto fallback_subject;
	}

	tmp = line;
	len = 0;
	while (*tmp && *tmp != ':') {
		tmp++;
		len++;
	}
	if (!*tmp) {
		log_error(LOG_ARGS, "No subject or invalid "
				"subject in digest listtext");
		goto fallback_subject;
	}
	tmp++;
	len++;
	if (strncasecmp(line, "Subject:", len) == 0) {
		tmp = unistr_utf8_to_header(tmp);
		subject = concatstr(2, "Subject:", tmp);
		myfree(tmp);
		myfree(line);

		/* Skip the empty line after the subject */
		line = get_processed_text_line(txt, 1, listaddr, listdelim,
				listdir);
		if (line == NULL || *line != '\0') {
			log_error(LOG_ARGS, "Too many headers "
					"in digest listtext");
			goto fallback_subject;
		}

		if (line != NULL) myfree(line);
		line = NULL;
	} else {
		log_error(LOG_ARGS, "No subject or invalid "
				"subject in digest listtext");
		goto fallback_subject;
	}

fallback_subject:
	if (subject == NULL) {
		if (lastindex == firstindex) {
			snprintf(buf, sizeof(buf), "%d", firstindex);
		} else {
			snprintf(buf, sizeof(buf), "%d-%d", firstindex, lastindex);
		}
		tmp = mystrdup(buf);
		snprintf(buf, sizeof(buf), "Digest of %s issue %d (%s)",
				listaddr, issue, tmp);
		subject = unistr_utf8_to_header(buf);
		myfree(tmp);
	}

	tmp = concatstr(10, "From: ", listname, listdelim, "help@", listfqdn,
			   "\nMIME-Version: 1.0"
			   "\nContent-Type: multipart/" DIGESTMIMETYPE "; "
			   "boundary=", boundary,
			   "\nSubject: ", subject,
			   "\n");

	myfree(listfqdn);
	myfree(subject);

	if (writen(fd, tmp, strlen(tmp)) < 0) {
		myfree(tmp);
		goto errdighdrs;
	}
	myfree(tmp);

	if(hdrfd >= 0 && dumpfd2fd(hdrfd, fd) < 0) {
		goto errdighdrs;
	}

	close(hdrfd);
	hdrfd = -1;

	if (writen(fd, "\n", 1) < 0) {
errdighdrs:
		log_error(LOG_ARGS, "Could not write digest headers to '%s'",
				queuename);
		close(fd);
		unlink(queuename);
		myfree(boundary);
		myfree(queuename);
		myfree(listaddr);
		myfree(listname);
		myfree(listdelim);
		if (txt != NULL) {
			close_text(txt);
			myfree(line);
		}
		if (hdrfd > 0) {
			close(hdrfd);
		}
		return -1;
	}

	if ((txt != NULL) && !statctrl(listdir, "nodigesttext")) {

		tmp = concatstr(3, "\n--", boundary,
				"\nContent-Type: text/plain; charset=UTF-8"
				"\nContent-Transfer-Encoding: 8bit"
				"\n\n");
		if (writen(fd, tmp, strlen(tmp)) == -1) {
			log_error(LOG_ARGS, "Could not write digest text/plain"
					" part headers to '%s'", queuename);
			close(fd);
			unlink(queuename);
			myfree(boundary);
			myfree(tmp);
			myfree(queuename);
			myfree(listaddr);
			myfree(listname);
			myfree(listdelim);
			if (txt != NULL) {
				close_text(txt);
				myfree(line);
			}
			return -1;
		}
		myfree(tmp);

		for (;;) {
			line = get_processed_text_line(txt, 0, listaddr, listdelim,
					listdir);
			if (line == NULL) break;
			len = strlen(line);
			line[len] = '\n';
			if(writen(fd, line, len+1) < 0) {
				myfree(line);
				log_error(LOG_ARGS, "Could not write"
						" std mail");
				break;
			}
			myfree(line);
		}

		finish_thread_list(tls);
		close_text(txt);
	} else if (txt != NULL) {
		finish_thread_list(tls);
		close_text(txt);
	}

	if (line != NULL) myfree(line);
	myfree(listaddr);
	myfree(listdelim);

	for (i=firstindex; i<=lastindex; i++) {
		snprintf(buf, sizeof(buf), "%d", i);
		
		archivename = concatstr(3, listdir, "/archive/", buf);
		archivefd = open(archivename, O_RDONLY);
		myfree(archivename);
		
		if (archivefd < 0)
			continue;
		
		tmp = concatstr(7, "\n--", boundary,
				"\nContent-Type: message/rfc822"
				"\nContent-Disposition: inline; filename=\"",
					listname, "_", buf, ".eml\""
				"\n\n");
		if (writen(fd, tmp, strlen(tmp)) == -1) {
			log_error(LOG_ARGS, "Could not write digest part "
					"headers for archive index %d to "
					"'%s'", i, queuename);
			close(fd);
			close(archivefd);
			unlink(queuename);
			myfree(boundary);
			myfree(tmp);
			myfree(queuename);
			myfree(listname);
			return -1;
		}
		myfree(tmp);

		if (dumpfd2fd(archivefd, fd) < 0) {
			log_error(LOG_ARGS, "Could not write digest part %d "
					"to '%s'", i,
					queuename);
			close(fd);
			close(archivefd);
			unlink(queuename);
			myfree(boundary);
			myfree(queuename);
			myfree(listname);
			return -1;
		}
		
		close(archivefd);
	}

	tmp = concatstr(3, "\n--", boundary, "--\n");
	if (writen(fd, tmp, strlen(tmp)) == -1) {
		log_error(LOG_ARGS, "Could not write digest end to '%s'",
				queuename);
		close(fd);
		unlink(queuename);
		myfree(boundary);
		myfree(queuename);
		myfree(listname);
		return -1;
	}

	close(fd);
	myfree(boundary);
	myfree(listname);
	myfree(tmp);

	childpid = fork();

	if(childpid < 0) {
		log_error(LOG_ARGS, "Could not fork");
		myfree(queuename);
		return -1;
	}

	if(childpid > 0) {
		do /* Parent waits for the child */
		      pid = waitpid(childpid, &status, 0);
		while(pid == -1 && errno == EINTR);
	} else {
		execlp(mlmmjsend, mlmmjsend,
				"-l", "7",
				"-L", listdir,
				"-m", queuename,
				(char *)NULL);
		log_error(LOG_ARGS, "execlp() of '%s' failed", mlmmjsend);
		exit(EXIT_FAILURE);  /* It is OK to exit, as this is a child */
	}

	myfree(queuename);

	return 0;
}