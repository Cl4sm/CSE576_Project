FILE *art_fd;
int bytes;
/*
 *  Convert an article to SLNP format
 */
{
#if 0
        int c;

        /* Write "rnews" line */
        if(fprintf(msg_fd, "#! rnews %d\n", bytes) < 0)
		return -1;

	/* Maintain block count */
	blk_cnt += (bytes + 64) / 128;

        /* Copy bytes */
        while (bytes--) {
                if( (c = getc (art_fd))==EOF) return(0);
		if( putc((0xff & c), msg_fd)==EOF) return(-1);
        }
        return 1;
#endif
	int	c, index = bytes;
	char	*buf;

	if((buf = (char *)malloc(bytes)) == NULL ) {
		fprintf(stderr, "could not allocate %i bytes for article\n", bytes);
		return -1;
	}
	if(fread(buf, 1, bytes, art_fd) != bytes) {
		fprintf(stderr, "could not fread() %i bytes from article\n", bytes);
		free(buf);
		return -1;
	}

        /* Determine proper index (may have to correct for \r\n lines) */
        for (c = 0; c < bytes-1; c++)
		if(buf[c] == '\r' && buf[c+1] == '\n')
			/* linetermination counts for
			   (and will be) a single byte! */
			index--;

       /* Write "rnews" line */
        if(fprintf(msg_fd, "#! rnews %d\n", index) < 0) {
		free(buf);
		return -1;
	}

        /* Copy bytes; be sure to enforce '\n' line termination */
        for (c = 0; c < bytes; c++)
		if(buf[c] != '\r' || (c >= bytes-1) || buf[c+1] != '\n')
			/* don't write the \r, whenever encountering \r\n */
			if( putc((0xff & buf[c]), msg_fd) == EOF) {
				free(buf);
				return -1;
			}

	free(buf);

	/* Maintain block count */
	blk_cnt += (bytes + 64) / 128;

        return 1;
}
