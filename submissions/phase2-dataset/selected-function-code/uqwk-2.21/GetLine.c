int GetLine (bytes, fd)
int *bytes;
FILE *fd;
/*
 *  Get a line from fd, put it in buf, check for end of message
 */
{
	int i;
	unsigned char eol;

	i = 0;

	eol = QWK_EOL;
	if (slnp_mode || zip_mode) eol = '\n';

	/* Read bytes until EOL or end of message */
	while (*bytes) {
		fread (&buf[i], 1, 1, fd);
		(*bytes)--;

		/* Lose CR's from ZipNews */
		if (buf[i] == '\r') buf[i] = 0;

		if ( (buf[i] == eol) || (i == BUF_LEN-1) ) {
			buf[i] = 0;
			return (1);
		}
		i++;
	}

	/* If we got here, we ran out of bytes */
	return (0);
}
