char *fn;
/*
 *  Process ZipNews mail reply
 */
{
	FILE *fd, *pfd;
	struct stat stat_buf;
	int c, have_cc, rval = 0;
	char *to, *cc, *addr;

	/* Get space for To: and Cc: */
	if (NULL == (to = (char *) malloc (BUF_LEN))) return (0);
	if (NULL == (cc = (char *) malloc (BUF_LEN))) return (0);

	/* Try to stat() it */
	if (0 != stat (fn, &stat_buf)) return (0);

	/* Try to open it */
	if (NULL == (fd = fopen (fn, "r"))) return(-1);

	/* Get To: and Cc: */
	GetHdr (fd, to, stat_buf.st_size, "To: ");
	have_cc = GetHdr (fd, cc, stat_buf.st_size, "Cc: ");

	/* Check for offline command */
	if (!strcasecmp (to, "UQWK")) {
		int ret = OffLine(fd, stat_buf.st_size);
		free (cc); free (to);
		fclose (fd);
		if (!read_only) unlink (fn);
		return (ret);
	}

	/* Make mailer command line */
	sprintf (buf, "%s '%s'", ZIP_MAILER, to);

	printf ("%s: Mailing to %s\n", progname, to);

	/* Open pipe to mailer */
	if (NULL == (pfd = popen (buf, "w"))) {
		fprintf (stderr, "%s: can't popen() mailer\n", progname);
		free (cc); free (to);
		fclose (fd);
		return (-1);
	}

	if( (fprintf (pfd, "%s\n", ZZZ) < 0) && ferror(pfd)) return(-1);

	/* Send bytes of message */
	while (EOF != (c = getc (fd))) {
		if (c != '\r') putc ((0xff & c), pfd);
	}

	/* Done */
	rval = pclose (pfd);

	/* Now do Cc: addresses */
	if (have_cc) {
		addr = strtok (cc, ", \t");

		while (addr != NULL) {
			/* Rewind file */
			fseek (fd, 0, 0);

			/* Make mailer command line */
			sprintf (buf, "%s '%s'", ZIP_MAILER, addr);

			printf ("%s:  Cc'ing to %s\n", progname, addr);

			/* Open pipe to mailer */
			if (NULL == (pfd = popen (buf, "w"))) {
				fprintf (stderr, "%s: can't popen() mailer\n",
						progname);
				fclose (fd);
				return(-1);
			}

			if( (fprintf (pfd, "%s\n", ZZZ) < 0) && ferror(pfd)) return(-1);

			/* Send bytes of message */
			while (EOF != (c = getc (fd))) {
				if (c != '\r') putc ((0xff & c), pfd);
			}

			/* Done */
			rval |= pclose (pfd);

			addr = strtok (NULL, ", \t");
		}
	}
	free (cc); free (to);

	fclose (fd);
	if (!read_only) unlink (fn);

	return (rval);
}
