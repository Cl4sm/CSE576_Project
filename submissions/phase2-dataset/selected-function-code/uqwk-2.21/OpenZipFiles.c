int OpenZipFiles ()
/*
 *  Open files for a ZipNews news packet
 */
{
	char fn[PATH_LEN];

	/* Open .nws file */
	sprintf (fn, "%s/%s.nws", home_dir, user_name);
	if (NULL == (nws_fd = fopen (fn, "w"))) {
		fprintf (stderr, "%s: can't open %s\n", progname, fn);
		return(-1);
	}

	/* Open .idx file */
	sprintf (fn, "%s/%s.idx", home_dir, user_name);
	if (NULL == (idx_fd = fopen (fn, "w"))) {
		fprintf (stderr, "%s: can't open %s\n", progname, fn);
		fclose (nws_fd);
		return(-1);
	}

	return (1);
}
