FILE *fd;
long bytes;
/*
 *  Look for Xref line.
 */
{
	long offset;
	int n;
	char *rc;

	/* Remember file position */
	offset = ftell (fd);

	/* Look through header */
	rc = Fgets (buf, BUF_LEN, fd);
	n = strlen (buf);
	while ( (rc != NULL) && (bytes > 0) && (n > 0) ) {
		/* Xref: line? */
		if (!strncmp (buf, "Xref: ", 6)) {
			/* Found one, process it */
			Xref (buf);

			/* Restore position, return */
			fseek (fd, offset, 0);
			return 1;
		}

		/* Get next line */
		bytes -= n;
		rc = Fgets (buf, BUF_LEN, fd);
		if (rc != NULL) n = strlen (buf);
	}

	/* Reposition file */
	fseek (fd, offset, 0);
	return 0;
}
