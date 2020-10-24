{
	FILE *fd;
	int bytes, ret = 0;
	char *to, cmd[PATH_LEN];
	long offset;

	/* Get space for To: */
	if (NULL == (to = (char *) malloc (BUF_LEN))) return (-1);

	/* Open the reply file */
	if (NULL == (fd = fopen (fn, "r"))) {
		fprintf (stderr, "%s: can't open %s\n", progname, fn);
		free (to);
		return(-1);
	}

	/* Read through it */
	while (0 != fread (buf, 4, 1, fd)) {

		/* Get byte count */
		bytes = buftoint((unsigned char *) buf);

		/* Remember file position */
		offset = ftell (fd);

		/* Find the To: line */
		GetHdr (fd, to, bytes, "To: ");

		/* Check for offline command */
		if (!strcmp (to, "uqwk") || !strcmp (to, "UQWK")) {
			if((ret=OffLine(fd, bytes)) == -1)
				break;
			continue;
		}

		/* Construct delivery line */
		sprintf (cmd, "%s", SLNP_MAILER);

		printf ("%s: Mailing to %s\n", progname, to);

		/* Pipe message to delivery agent */
		ret = SLPipe(fd, bytes, cmd);
	}
	free (to);

	fclose (fd);
	return (ret);
}
