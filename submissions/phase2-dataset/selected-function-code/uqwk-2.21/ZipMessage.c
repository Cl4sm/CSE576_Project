FILE *          fd;
struct mail_ent *mp;
char		*xhdrs;
{
	int bytes, c;

	/* Compute message size */
	bytes = mp->end - mp->begin;

	/* Write separator */
	for (c=0; c<20; c++)
		if( putc(1, mai_fd) == EOF) {
			fprintf(stderr,"%s: write error!\n",progname);
			exit(-1);
		}

	if(	(fprintf (mai_fd, "\r\n") < 0)
		&& ferror(mai_fd) ) {
		fprintf(stderr,"%s: write error!\n",progname);
		exit(-1);
	}

	/* Seek to start of message */
	fseek (fd, mp->begin, 0);

	if(xhdrs && *xhdrs) {
		char s[1024] = "";
		int  err=0;

		while(fgets(s, sizeof(s), fd) && ftell(fd) < mp->end) {
			bytes -= strlen(s);
			if(s[0]=='\n' || (s[0]=='\r' && s[1]=='\n')) {
				if(fputs(xhdrs,msg_fd)==EOF
				|| fputs(s,    msg_fd)==EOF)
					err++;
				break;
			}
			if(fputs(s, msg_fd)==EOF) {
				err++;
				break;
			}
		}
		if(err) {
			fprintf(stderr, "%s: write error!\n", progname);
			exit(-1);
		}
	}

	/* Copy bytes */
	while (bytes--) {
		if( (c = getc(fd)) == EOF) {
			fprintf(stderr,"%s: read error (%d bytes unread)\n", progname, bytes);
			exit(-1);
		}

		/* ZipNews doesn't like ^Z's */
		if (c == 26) c = 32;

		/* Map LF to CRLF */
		if ( c==10 && putc(13, mai_fd) == EOF) {
			fprintf(stderr,"%s: write error!\n",progname);
			exit(-1);
		}

		if( putc((0xff & c), mai_fd) == EOF) {
			fprintf(stderr,"%s: write error!\n",progname);
			exit(-1);
		}
	}
}
