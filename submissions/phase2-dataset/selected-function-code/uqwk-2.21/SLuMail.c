{
	FILE *fd;
	int bytes, ret = 0;
	char *to, cmd[PATH_LEN];
	long offset;

	/* Get space for To: */
	if (NULL == (to = (char *) malloc (BUF_LEN))) OutOfMemory();

	/* Open the reply file */
	if (NULL == (fd = fopen (fn, "r"))) {
		fprintf (stderr, "%s: can't open %s\n", progname, fn);
		free (to);
		return(-1);
	}

	/* Read through it */
	while (NULL != Fgets (buf, BUF_LEN, fd)) {
		if (strncmp (buf, "#! rnews ", 9)) {
			fprintf (stderr, "%s: malformed reply file\n",
					progname);
			fclose (fd);
			free (to);
			return (-1);
		}

		/* Get byte count */
		sscanf (&buf[8], "%d", &bytes);

		/* Remember file position */
		offset = ftell (fd);

		/* Look for To: */
		GetHdr (fd, to, bytes, "To: ");

		/* Check for offline command */
		if (!strcasecmp (to, "UQWK")) {
			if((ret = OffLine (fd, bytes)) == -1)
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
