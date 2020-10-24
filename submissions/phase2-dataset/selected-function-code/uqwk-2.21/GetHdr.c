FILE *fd;
char *cc, *hdr;
int bytes;
/*
 *  Find given header line
 */
{
	int offset, n, cnt;
	char *rc;

	cnt = strlen (hdr);

	/* Remember file position */
	offset = ftell (fd);

	/* Look through header */
	rc = Fgets (buf, BUF_LEN, fd);
	n = strlen (buf);
	while ( (rc != NULL) && (bytes > 0) && (n > 0) ) {
		/* Right line? */
		if (!strncmp (buf, hdr, cnt)) {
			strcpy (cc, &buf[cnt]);
			fseek (fd, offset, 0);
			return (1);
		}

		/* Get next line */
		bytes -= n;
		rc = Fgets (buf, BUF_LEN, fd);
		if (rc != NULL) n = strlen (buf);
	}

	/* Reposition file */
	fseek (fd, offset, 0);
	return (0);
}
