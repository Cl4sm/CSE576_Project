int DoReply ()
{
	int n, rep_cnt, ret;
	char bbs[PATH_LEN];

	/* Check for ZipNews reply */
	if (zip_mode)
		return DoZipReplies();

	rep_cnt = 0;

	/* Open the packet */
	if (NULL == (rep_fd = fopen (rep_file, "r"))) {
		fprintf (stderr, "%s: can't open %s\n", progname, rep_file);
		return(-1);
	}

	/* Handle SLNP reply */
	if (slnp_mode) {
		if((ret=SLNPReply()))
			return ret;

		fclose (rep_fd);
		return 0;
	}

	/* Get the first block, the BBS ID */
	if (1 != fread (buf, 128, 1, rep_fd)) {
		fprintf (stderr, "%s: reply packet read error\n", progname);
		fclose (rep_fd);
		return -1;
	}

	/* Extract bbs id and check */
	sscanf (bbs_id, "%*d,%s", bbs);
	n = strlen (bbs);
	buf[n] = 0;
	if (strcmp (bbs, buf)) {
		fprintf (stderr, "%s: reply BBS ID mismatch: %s != %s\n",
			progname, buf, bbs);
		fclose (rep_fd);
		return 0;
	}

	/* Read the .newsrc file; we will need the list of conferences */
	if((ret=ReadNewsrc()<=0))
		return(ret);

	/* Read the next message header and process it */
	while (1 == fread (&rep_hdr, 128, 1, rep_fd)) {
		if((ret=SendReply()) <= 0) return(ret);
		rep_cnt++;
	}

	fclose (rep_fd);
	printf ("%s: sent %d replies\n", progname, rep_cnt);

	return 1;
}
