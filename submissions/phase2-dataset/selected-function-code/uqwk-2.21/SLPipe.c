int SLPipe (fd, bytes, agent)
FILE *fd;
int bytes;
char *agent;
/*
 *  Pipe a SLNP reply to the specified delivery agent
 */
{
	FILE *pfd;
	int c;

	/* Open pipe to agent */
	if (NULL == (pfd = popen (agent, "w"))) {
		fprintf (stderr, "%s: can't open reply pipe (%s)\n", progname, agent);
		while (bytes--) getc (fd);
		return(-1);
	}

	/* Send message to pipe */
	while (bytes--) {
		if( (c = getc(fd) & 0xff) == EOF) {
			fprintf(stderr,"%s: unable to read %d bytes\n",
			        progname, bytes);
			return -1;
		}
		if( putc(c, pfd) == EOF) {
			fprintf(stderr, "%s: write error\n", progname);
			return -1;
		}
	}

	return(pclose(pfd));
}
