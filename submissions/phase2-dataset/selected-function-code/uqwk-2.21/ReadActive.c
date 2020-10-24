/*
 *  Read active file
 */
{
	struct act_ent *ap;
	char group_name[PATH_LEN];

	/* Don't bother if it's already here */
	if (act_list != NULL) return (1);

#if !defined(NNTP)
	/* Open the active file */
	if (NULL == (act_fd = fopen (act_file, "r"))) {
		fprintf (stderr, "%s: can't open %s\n", progname, act_file);
		return(-1);
	}
#else
	/* Retrieve active file from news server */
	if (NULL == (act_fd = (FILE *)getactive_nntp())) {
		fprintf (stderr, "%s: can't get active file\n", progname);
		return (-1);
	}
#endif

	/* Read through the full active file */
	while (NULL != Fgets (buf, BUF_LEN, act_fd)) {
		/* Get new act entry */
		ap = (struct act_ent *) malloc (sizeof (struct act_ent));
		if (ap == NULL) OutOfMemory();

		/* Parse name, message numbers */
		sscanf (buf, "%s %d %d", group_name, &ap->hi, &ap->lo);
		ap->name = (char *) malloc (1+strlen(group_name));
		if (ap->name == NULL) OutOfMemory();
		strcpy (ap->name, group_name);

		/* Add to list */
		ap->next = act_list;
		act_list = ap;
	}
	fclose (act_fd);

	return (1);
}
