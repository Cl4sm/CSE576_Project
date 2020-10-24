int SLbNews (char *fn)
{
	FILE *fd;
	int  bytes, rval=0;
	char *grp;

	/* Get space for newsgroup name */
	if (NULL == (grp = (char *) malloc (BUF_LEN))) {
		OutOfMemory();
		return (-1);
	}

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

	/* Open the reply file */
	if (NULL == (fd = fopen (fn, "r"))) {
		fprintf (stderr, "%s: can't open %s\n", progname, fn);
		return(-1);
	}

	/* Read through it */
	while (0 != fread (buf, 4, 1, fd)) {

		/* Get byte count */
		bytes = buftoint((unsigned char *) buf);

		if (GetHdr (fd, grp, bytes, "Newsgroups: ")) {
			printf ("%s: Posting article to %s\n", progname, grp);
		}

		/* Pipe message to delivery agent */
		rval = SLPipe(fd, bytes, posting_agent);
	}
	free (grp);
	fclose (fd);
	return (rval);
}
