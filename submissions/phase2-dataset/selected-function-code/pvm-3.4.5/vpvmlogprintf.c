int
vpvmlogprintf(fmt, ap)
	char *fmt;
	va_list ap;
{
	char vtmp[255];
	int cnt = 0;
	int cc;

#ifdef	TIMESTAMPLOG
	time_t now;
	struct tm *tmp;

	time(&now);
	tmp = localtime(&now);
#endif	/*TIMESTAMPLOG*/

	/* snag var arg string, in case re-use whacks it... */
	/* (thank you very much x86_64...  :-Q */
	vsprintf(vtmp, fmt, ap);

	if (log_how & 1) {
		if (atnewline) {
			if (pvmmytid)
				fprintf(stderr, "[t%x] ", pvmmytid);
			else
				fprintf(stderr, "[pvmd pid%d] ", pvmmyupid);

#ifdef	TIMESTAMPLOG
			fprintf(stderr, "%02d/%02d %02d:%02d:%02d ",
						tmp->tm_mon + 1,
						tmp->tm_mday,
						tmp->tm_hour,
						tmp->tm_min,
						tmp->tm_sec);
#endif	/*TIMESTAMPLOG*/
		}
		/* cc = vfprintf(stderr, fmt, ap); */
		cc = fprintf(stderr, vtmp);
		cnt = ( cc >= 0 ) ? cnt + cc : cc;
		fflush(stderr);
	}

	if (log_how & 2) {
		if (log_alrdy < pvmdlogmax) {
			if (atnewline) {
				if (pvmmytid)
					fprintf(log_ff, "[t%x] ", pvmmytid);
				else
					fprintf(log_ff, "[pvmd pid%d] ", pvmmyupid);

#ifdef	TIMESTAMPLOG
				fprintf(log_ff, "%02d/%02d %02d:%02d:%02d ",
						tmp->tm_mon + 1,
						tmp->tm_mday,
						tmp->tm_hour,
						tmp->tm_min,
						tmp->tm_sec);
#endif	/*TIMESTAMPLOG*/
			}

			/* cc = vfprintf(log_ff, fmt, ap); */
			cc = fprintf(log_ff, vtmp);
			cnt = ( cnt >= 0 ) ? ( ( cc >= 0 ) ? cnt + cc : cc ) : cnt;
			fflush(log_ff);

			if ((log_alrdy += cnt) >= pvmdlogmax)
				(void)write(log_fd, toomuch, strlen(toomuch));
		}
	}

	atnewline = (fmt[strlen(fmt) - 1] == '\n') ? 1 : 0;

/*
	if ((log_how & 4) && hosts->ht_local != hosts->ht_cons) {
XXX send error-log message to console host
	}
*/

	return(cnt);
}
