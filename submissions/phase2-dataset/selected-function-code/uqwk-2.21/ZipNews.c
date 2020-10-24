int ZipNews (fn)
char *fn;
/*
 *  Process ZipNews news reply
 */
{
	FILE *fd, *pfd;
	struct stat stat_buf;
	int c, rval = 0;

	/* Try to stat() it */
	if (0 != stat (fn, &stat_buf)) return (0);

#ifdef	GNKSA
	if(gnksa_level &&
	   GnksaErrors(GnksaCheckFile(fn, fn, gnksa_level&GNKSA_VERBOSE, FROMOPTIONAL))) {
		AppendBadArticle(fn);
		if(gnksa_level & GNKSA_HONOUR) {
			fprintf (stderr, "%s: cannot post invalid message\n", progname);
			return(-1);
		} else
			fprintf (stderr, "%s: warning: attempting to post invalid message\n", progname);
	}
#endif

	/* Try to open it */
	if (NULL == (fd = fopen (fn, "r"))) return(-1);


	printf ("%s: Posting article...\n", progname);

	/* Open pipe to inews */
	if (NULL == (pfd = popen (posting_agent, "w"))) {
		fprintf (stderr, "%s: can't popen() inews\n", progname);
		fclose (fd);
		return(-1);
	}

	if( (fprintf (pfd, "%s\n", ZZZ) < 0) && ferror(pfd)) return(-1);

	/* Send bytes of message */
	while (EOF != (c = getc (fd))) {
		if (c != '\r') putc ((0xff & c), pfd);
	}

	/* Done */
	rval = pclose (pfd);
	fclose (fd);
	if (!read_only) unlink (fn);

	return (rval);
}
