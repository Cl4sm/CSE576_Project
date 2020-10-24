void InitStuff()
{
	buf = malloc(BUF_LEN);
	if (!buf)
		OutOfMemory();

	char msg_fname[PATH_LEN];

	/* Mail, conference, etc. lists */
	mail_list = NULL;
	conf_list = NULL;
	last_conf = NULL;
	act_list = NULL;
	nrc_list = NULL;
	trn_list = NULL;
	ng_list = NULL;

	/* Message and conference counts */
	msg_cnt = 0;
	conf_cnt = 0;

	/* Kludge around fclose() bug in close.c */
	msg_fd = NULL;

	/* Open MESSAGES.DAT */
	if (!slnp_mode && !zip_mode && !sum_mode) {
		strcpy (msg_fname, home_dir);
		strcat (msg_fname, "/");
		strcat (msg_fname, "messages.dat");

		if (NULL == (msg_fd = fopen (msg_fname, "w"))) {
			fprintf (stderr, "%s: can't open %s\n",
				progname, msg_fname);
			exit(-1);	/* Return failure errorcode to caller */
		}

		/* Write magic string to MESSAGES.DAT */
		if(	(fprintf(msg_fd, "%-128s", QWK_MAGIC) < 0) /* != 128? */
			&& ferror(msg_fd) ) {
			fprintf(stderr,"%s: write error!\n",progname);
			exit(-1);
		}
		blk_cnt = 2;
	}

	/* Open summary file */
	if (sum_mode) {
		if (NULL == (sum_fd = fopen (sum_file, "w"))) {
			fprintf (stderr, "%s: can't open %s\n",
					progname, sum_file);
			exit(-1);	/* Return failure errorcode to caller */
		}
	}

	/* Read the ng trans table if specified */
	if (strcmp (trn_file, DEF_TRN_FILE)) ReadTrans();

	/* Read the desired newsgroups file if specified */
	if (strcmp (ng_file, DEF_NG_FILE)) ReadNG();

#ifdef NNTP
/* JS -- no need to open a connection if replying only!
	if (strcmp (rep_file, DEF_REP_FILE) || do_news) connect_nntp();
*/
	if (do_news) connect_nntp();

#endif
}
