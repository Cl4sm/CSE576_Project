FILE *art_fd;
int bytes;
struct nrc_ent *np;
/*
 *  Convert article to ZipNews format
 */
{
	int c;
	long offset;

	/* Write separator */
	for (c=0; c<20; c++) if(putc(1, nws_fd)==EOF) return(-1);
	if((fprintf(nws_fd, "\r\n") < 0) && ferror(nws_fd)) return(-1);

	/* Write index file entry for this group if there isn't
	   already one */
	if (!zip_flag) {
		offset = ftell (nws_fd);
		if(	(fprintf(idx_fd, "N %08ld %s\r\n", offset, np->name) < 0)
			&& ferror(idx_fd) ) return(-1);
		zip_flag = 1;
	}

	/* Maintain block count */
	blk_cnt += (bytes + 64) / 128;

        /* Copy bytes */
        while (bytes--) {
                if( (c = getc(art_fd))==EOF) return(-1);

		/* ZipNews doesn't like ^Z's */
		if (c == 26) c = 32;

		/* Map LF to CRLF */
		if( c==10 && putc(13, nws_fd)==EOF)
			return -1;

                if( putc((0xff & c), nws_fd)==EOF)
			return -1;
        }
        return 1;
}
