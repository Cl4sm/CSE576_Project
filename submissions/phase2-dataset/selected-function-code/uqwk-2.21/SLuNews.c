int SLuNews (fn)
char *fn;
/*
 *  Process a SLNP news reply file, usenet type
 */
{
	FILE *fd;
	int bytes, rval = 0;
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
	while (NULL != Fgets (buf, BUF_LEN, fd)) {
		if (strncmp (buf, "#! rnews ", 9)) {
			fprintf (stderr, "%s: mailformed reply file\n",
					progname);
			fclose (fd);
			return(-1);
		}

		/* Get byte count */
		sscanf (&buf[8], "%d", &bytes);

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
