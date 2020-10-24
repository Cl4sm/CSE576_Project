/*
 *  Process replies in a Zip packet
 */
{
	int n;
	char fn[PATH_LEN];

	if ((n=ZipId()) <=0) return(n);

	/* Loop through possible mail files.  This is a little kludgy,
	   but readdir() seems to have problems on some systems,
	   including Esix, which is the system I use. */
	for (n=0; n<100; n++) {
		/* Construct file name */
		sprintf (fn, "%s/%s.m%02d", rep_file, user_name, n);

		/* Process it */
		if((n=ZipMail(fn)) <= 0) return(n);
	}

	/* Loop through possible news files */
	for (n=0; n<100; n++) {
		/* Construct file name */
		sprintf (fn, "%s/%s.n%02d", rep_file, user_name, n);

		/* Process it */
		if((n=ZipNews(fn)) <= 0) return(n);
	}
	return (1);
}
