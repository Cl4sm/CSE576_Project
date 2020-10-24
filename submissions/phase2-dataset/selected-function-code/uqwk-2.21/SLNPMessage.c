FILE            *fd;
struct mail_ent *mp;
char		*xhdrs;
{
	long bytes, c, b;

	/* Compute message size */
	b = bytes = mp->end - mp->begin;
	/* If there are xtra heades to be added, add those to the byte count */
	if(xhdrs)
		b += strlen(xhdrs);

	/* Write byte count line */
	for (c=3; c>=0; c--) {
		buf[c] = b % 256;
		b = b / 256;
	}
	if(fwrite(buf, 4, 1, msg_fd) != 1) {
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

	/* Copy rest of bytes */
	while (bytes--) {
		if((c = getc(fd)) == EOF) {
			fprintf(stderr,"%s: read error (%ld bytes unread)\n", progname, bytes+1);
			exit(-1);
		}
		if(putc((0xff & c), msg_fd) == EOF) {
			fprintf(stderr,"%s: write error!\n",progname);
			exit(-1);
		}
	}
}
