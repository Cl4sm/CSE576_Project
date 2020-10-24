FILE *pfd;
{
	int c;
	FILE *cfd;

	if (strlen(buf) < 7) {
		fprintf (pfd, "Usage: SHELL command\n");
		return (0);
	}

	if (NULL == (cfd = popen (&buf[6], "r"))) {
		fprintf (pfd, "Can't open pipe for command\n");
		return (-1);
	}

	while (EOF != (c = getc (cfd))) {
		if(putc( (0xff & c), pfd) == EOF)
			return(-1);
	}
	if(putc ('\n', pfd) == EOF)
		return(-1);
	pclose (cfd);
	return (1);
}
