int _CloseStuff()
{
#ifdef NNTP
	/*  JS -- server might have been connected to implicitly! */
	close_server();
#endif

	if (msg_fd != NULL) fclose (msg_fd);

	/* Write QWK CONTROL.DAT file (or SLNP AREAS file) */
	if (!zip_mode && !sum_mode && (do_mail || do_news)
	&&  WriteControl() < 0)
		return -1;

	/* Close summary file */
	if (sum_mode) fclose (sum_fd);

	/* Update .newsrc */
	if (do_news && (!read_only) && WriteNewsrc() < 0)
		return(-2);

	if ( (blk_cnt >= max_blks) && (max_blks > 0) ) {
		fprintf (stderr,
			"%s: block count exceeded; some articles not packed\n",
			progname);
	}

	/* Remove reply packet */
	if ( (!read_only) && (strcmp (rep_file, DEF_REP_FILE)))
		unlink (rep_file);

	free(buf);
	buf = NULL;

	return 0;
}
