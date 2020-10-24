FILE *          fd;
struct mail_ent *mp;
char		*xhdrs;
{
	struct qwk_hdr hdr;
	char c[PATH_LEN], *eof, ndx[5];
	int out_bytes, n, i;

	/* Write the ndx file entry */
	inttoms (blk_cnt, (unsigned char *) ndx);
	ndx[4] = conf_cnt;
	if(fwrite (ndx, 5, 1, ndx_fd) != 1) {
		fprintf(stderr,"%s: write error!\n",progname);
		exit(-1);
	}

	Spaces ((char *)&hdr, 128);

	/* Fill in the header fields we can do now */
	hdr.status = QWK_PRIVATE;
	PadNum (msg_cnt, hdr.number, 7);
	Spaces (hdr.password, 12);
	Spaces (hdr.refer, 8);
	hdr.flag = (char) QWK_ACT_FLAG;
	IntNum (conf_cnt, hdr.conference);
	IntNum (msg_cnt+1, hdr.msg_num);
	hdr.tag = ' ';

	msg_cnt++;

	/* Seek to start of message */
	fseek (fd, mp->begin, 0);

	/* Read the From line */
	Fgets (buf, BUF_LEN, fd);

	/* The second field of the From line is used as a first
	   guess for who sent the message */
	sscanf (&buf[5], "%s", c);
	PadString (c, hdr.from, 25);

	/* Now read through header lines, looking for ones we need */
	eof = Fgets (buf, BUF_LEN, fd);
	while ( (0 != strlen(buf)) && (eof != NULL) ) {
		if (!strncmp (buf, "Date: ", 6)) {
			ParseDate (&buf[6], &hdr);
		} else if (!strncmp (buf, "To: ", 4)) {
			PadString (&buf[4], hdr.to, 25);
		} else if (!strncmp (buf, "Subject: ", 9)) {
			PadString (&buf[9], hdr.subject, 25);
		} else if (!strncmp (buf, "From: ", 6)) {
			PadString (ParseFrom(&buf[6]), hdr.from, 25);
		}
		eof = Fgets (buf, BUF_LEN, fd);
	}
	mp->text = ftell(fd);

	/* Fill in block count */
	if (inc_hdrs) {
		PadNum (2+(mp->end-mp->begin)/128, hdr.blocks, 6);
		blk_cnt += (1+(mp->end - mp->begin)/128);
	} else {
		PadNum (2+(mp->end-mp->text)/128, hdr.blocks, 6);
		blk_cnt += (1+(mp->end - mp->text)/128);
	}

	/* Write out the message header */
	if( fwrite(&hdr, 128, 1, msg_fd) != 1) {
		fprintf(stderr,"%s: write error!\n",progname);
		exit(-1);
	}
	blk_cnt++;

	/* Now write the message text */
	if (inc_hdrs) fseek(fd, mp->begin, 0);
	out_bytes = 0;

	eof = Fgets (buf, BUF_LEN, fd);
	do {
		n = strlen (buf);

		/* MMDF puts funny things in messages -- change to spaces */
		for (i=0; i<n; i++) {
			if (buf[i] == 1) buf[i] = ' ';
			if (buf[i] == 0) buf[i] = ' ';
		}

		if( fwrite(buf, 1, n, msg_fd) != n) {
			fprintf(stderr,"%s: write error!\n",progname);
			exit(-1);
		}
		out_bytes += n;
		if (n < BUF_LEN-1) {
			if( putc (QWK_EOL, msg_fd) == EOF) {
				fprintf(stderr,"%s: write error!\n",progname);
				exit(-1);
			}
			out_bytes++;
		}
		eof = Fgets (buf, BUF_LEN, fd);
	} while ( (strncmp(buf,"From ", 5)) && (NULL != eof) );

	/* Pad block as necessary */

	n = out_bytes % 128;
	for (;n<128;n++)
		if( putc(' ', msg_fd) == EOF) {
		fprintf(stderr,"%s: write error!\n",progname);
		exit(-1);
	}
}
