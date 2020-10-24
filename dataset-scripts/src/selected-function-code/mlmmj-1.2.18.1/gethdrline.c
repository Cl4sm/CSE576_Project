char *gethdrline(int fd)
{
	char *line = NULL, *retstr = NULL, *oldretstr = NULL;
	char ch;
	ssize_t n;
	
	retstr = mygetline(fd);
	if (!retstr) {
		return NULL;
	}

	/* do not attempt to unfold the end-of-headers marker */
	if (retstr[0] == '\n')
		return retstr;
	
	for(;;) {
		/* look-ahead one char to determine if we need to unfold */
		n = readn(fd, &ch, 1);
		if (n == 0) {  /* end of file, and therefore also headers */
			return retstr;
		} else if (n == -1) {  /* error */
			log_error(LOG_ARGS, "readn() failed in gethdrline()");
			myfree(retstr);
			return NULL;
		}

		if (lseek(fd, -1, SEEK_CUR) == (off_t)-1) {
			log_error(LOG_ARGS, "lseek() failed in gethdrline()");
			myfree(retstr);
			return NULL;
		}

		if ((ch != '\t') && (ch != ' '))  /* no more unfolding */
			return retstr;

		oldretstr = retstr;
		line = mygetline(fd);
		if (!line) {
			return retstr;
		}

		retstr = concatstr(2, oldretstr, line);
		
		myfree(oldretstr);
		myfree(line);
	}
}