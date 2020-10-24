int DoArticle (news_path, artnum, np, cp)
char *news_path;
int artnum;
struct nrc_ent *np;
struct conf_ent *cp;
{
	struct qwk_hdr hdr;
	struct stat stat_buf;
	long txt_offset, end_offset;
	int n, out_bytes;
	char ndx[5], *eof;
	FILE *art_fd;
	char art_file[PATH_LEN];

#ifdef NNTP
	/* retrieve article from news server */
	art_fd = nntpopen(artnum, GET_ARTICLE);
	if (art_fd == NULL){
#ifdef DEBUG
		char *s = ignore_0fd?"(ignored)":"(stop)";
		fprintf( stderr, "%s: couldn't nntpopen article#%d %s\n", progname, artnum, s );
#endif
		return ignore_0fd ? 1 : 0;
	}

	/* Construct article's file name */
	sprintf(art_file,"/tmp/rrn%ld.%ld", (long) artnum, (long) getpid());
#else
	/* Construct article's file name */
	sprintf (art_file, "%s%d", news_path, artnum);

	/* Forget it if we can't open the article */
	if (NULL == (art_fd = fopen (art_file, "r"))) {
#ifdef DEBUG
		fprintf(stderr,"%s: couldn't read article#%d\n", progname, artnum);
#endif
		return 0;
	}

#endif
	/* stat() the article to get file size */
	if (0 != stat (art_file, &stat_buf)) {
		fclose (art_fd);
#ifdef DEBUG
		fprintf(stderr,"%s: couldn't stat article %d\n", progname, artnum);
#endif
#ifdef NNTP
		unlink (art_file);
#endif
		return 0;
	}

	end_offset = stat_buf.st_size;

	/* Skip empty articles */
	if (end_offset == 0) {
		fclose (art_fd);
#ifdef NNTP
		unlink (art_file);
#endif
#ifdef DEBUG
		fprintf(stderr,"%s: empty article\n", progname);
#endif
		return (0);
	}

#ifdef SCORES
        if (do_kills) {
	        if (Kill(art_fd) < group_kill_thresh) {
		        fclose (art_fd);
#ifdef NNTP
		        unlink (art_file);
#endif
		        return (-1);
		} else {
		        fseek (art_fd, 0L, SEEK_SET);
		}
	}
#endif

	/* We now assume the article is for real, so we can
	   bump this conference's article count */
	cp->count++;

	/* Check the Xref line if desired */
	if (xrf_mode) DoXref (art_fd, end_offset);

	/* Do SLNP stuff */
	if (slnp_mode) {
		if((n=SLNPArticle(art_fd, end_offset))!=1) return(n);
		fclose (art_fd);
#ifdef NNTP
		unlink(art_file);
#endif
		return (1);
	}

	/* Do ZipNews stuff */
	if (zip_mode) {
		n=ZipArticle(art_fd, end_offset, np);
		fclose (art_fd);
#ifdef NNTP
		unlink(art_file);
#endif
		return (n);
	}

	/* Do summary stuff */
	if (sum_mode) {
		SumArticle (art_fd, artnum, end_offset, np);
		fclose (art_fd);
#ifdef NNTP
		unlink(art_file);
#endif
		return (1);
	}

	/* Write the index file entry */
	inttoms (blk_cnt, (unsigned char *)ndx);
	ndx[4] = np->conf;
	if(fwrite(ndx, 5, 1, ndx_fd)!=1) return(-2);

	Spaces ((char *)&hdr, 128);

	/* Fill in some header fields */
	hdr.status = QWK_PUBLIC;
	PadNum (msg_cnt, hdr.number, 7);
	Spaces (hdr.password, 12);
	Spaces (hdr.refer, 8);
	hdr.flag = (char) QWK_ACT_FLAG;
	IntNum (np->conf, hdr.conference);
	IntNum (msg_cnt+1, hdr.msg_num);
	hdr.tag = ' ';
	PadString ("ALL", hdr.to, 25);

	msg_cnt++;

	/* Process header lines */
	eof = Fgets (buf, BUF_LEN, art_fd);
	while ( (0 != strlen(buf)) && (eof != NULL) ) {
		if (!strncmp (buf, "Date: ", 6)) {
			ParseDate (&buf[6], &hdr);
		} else if (!strncmp (buf, "Subject: ", 9)) {
			PadString (&buf[9], hdr.subject, 25);
		} else if (!strncmp (buf, "From: ", 6)) {
			PadString (ParseFrom(&buf[6]), hdr.from, 25);
		}
		eof = Fgets (buf, BUF_LEN, art_fd);
	}

	txt_offset = ftell (art_fd);

	/* Compute block count */
	if (inc_hdrs) {
		PadNum (2+end_offset/128, hdr.blocks, 6);
		blk_cnt += 1+end_offset/128;
	} else {
		PadNum (2+(end_offset-txt_offset)/128, hdr.blocks, 6);
		blk_cnt += 1+(end_offset-txt_offset)/128;
	}

	/* Write the message header */
	if((out_bytes =fwrite(&hdr, 128, 1, msg_fd)) != 1 ) {
		return(-2);
	}

	blk_cnt++;

	/* Now write the article's text */
	if (inc_hdrs) fseek (art_fd, 0, 0);
	out_bytes = 0;

	while (NULL != Fgets (buf, BUF_LEN, art_fd))
	{
		n = strlen (buf);
		if(fwrite(buf, 1, n, msg_fd)!=n) {
			return(-2);
		}
		out_bytes += n;

		if (n < BUF_LEN-1) {
			if(putc(QWK_EOL, msg_fd) == EOF) {
				 return(-2);
			}
			out_bytes++;
		}
	}

	/* Pad block as necessary */
	n = out_bytes % 128;
	for (;n<128;n++) putc (' ', msg_fd);

	fclose (art_fd);
#ifdef NNTP
	unlink(art_file);
#endif

	return (1);
}
