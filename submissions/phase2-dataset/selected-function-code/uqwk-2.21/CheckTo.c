int CheckTo (to, bytes)
char *to;
int bytes;
/*
 *  Check the first line of the body of the message for a To: line.
 *  This is the only way to send to addresses over 25 characters.
 *
 *  Whether we find a To: line or not, we have to leave the file
 *  positioned right where it is.
 */
{
	long offset;
	unsigned char c;
	int i;

	/* Sanity check */
	if (bytes < 5) return (0);

	offset = ftell (rep_fd);

	/* Check first four bytes */
	fread (buf, 4, 1, rep_fd);
	bytes -= 4;
	if (strncmp (buf, "To: ", 4)) {
		/* Doesn't match */
		fseek (rep_fd, offset, 0);
		return (0);
	}

	/* Copy in the rest of the line until white space, EOL,
	   or run out of bytes */
	i = 0;
	fread (&c, 1, 1, rep_fd);
	bytes--;

	while ( (bytes >= 0) && (c != QWK_EOL) &&
	        (c != 9) && (c != ' ') ) {
		to[i++] = c;

		fread (&c, 1, 1, rep_fd);
		bytes--;
	}
	to[i] = 0;

	/* Done! */
	fseek (rep_fd, offset, 0);
	return (1);
}
