	char *s;	/* text */
{
	pvmlogprintf("%s", s);
	atnewline = 1;
	return 0;

#if NOMORELOGERROR
	int len = strlen(s);
	char buf[40];
	int l;
#ifdef	TIMESTAMPLOG
	time_t now;
	struct tm *tmp;

	time(&now);
	tmp = localtime(&now);
#endif	/*TIMESTAMPLOG*/

	if (log_how & 1) {
#ifdef	TIMESTAMPLOG
		if (pvmmytid)
			fprintf(stderr, "[t%x] %02d/%02d %02d:%02d:%02d %s", pvmmytid,
					tmp->tm_mon + 1,
					tmp->tm_mday,
					tmp->tm_hour,
					tmp->tm_min,
					tmp->tm_sec,
					s);
		else
			fprintf(stderr, "[pvmd pid%d] %02d/%02d %02d:%02d:%02d %s", pvmmyupid,
					tmp->tm_mon + 1,
					tmp->tm_mday,
					tmp->tm_hour,
					tmp->tm_min,
					tmp->tm_sec,
					s);

#else	/*TIMESTAMPLOG*/
		if (pvmmytid)
			fprintf(stderr, "[t%x] %s", pvmmytid, s);
		else
			fprintf(stderr, "[pvmd pid%d] %s", pvmmyupid, s);
#endif	/*TIMESTAMPLOG*/
	}

	if (log_how & 2) {
		if (log_alrdy < pvmdlogmax) {
#ifdef	TIMESTAMPLOG
			if (pvmmytid)
				sprintf(buf, "[t%x] %02d/%02d %02d:%02d:%02d ", pvmmytid,
						tmp->tm_mon + 1,
						tmp->tm_mday,
						tmp->tm_hour,
						tmp->tm_min,
						tmp->tm_sec);
			else
				sprintf(buf, "[pvmd pid%d] %02d/%02d %02d:%02d:%02d ", pvmmyupid,
						tmp->tm_mon + 1,
						tmp->tm_mday,
						tmp->tm_hour,
						tmp->tm_min,
						tmp->tm_sec);

#else	/*TIMESTAMPLOG*/
			if (pvmmytid)
				sprintf(buf, "[t%x] ", pvmmytid);
			else
				sprintf(buf, "[pvmd pid%d] ", pvmmyupid);
#endif	/*TIMESTAMPLOG*/

			l = strlen(buf);
			(void)write(log_fd, buf, l);
			(void)write(log_fd, s, len);
			if ((log_alrdy += len + l) >= pvmdlogmax)
				(void)write(log_fd, toomuch, strlen(toomuch));
		}
	}

/*
	if ((log_how & 4) && hosts->ht_local != hosts->ht_cons) {
XXX send error-log message to console host
	}
*/
#endif
}
