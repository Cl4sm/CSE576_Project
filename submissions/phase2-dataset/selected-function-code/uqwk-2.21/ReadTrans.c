/*
 *  Read the newsgroup name translation table
 */
{
	FILE *trn_fd;
	struct trn_ent *tp;
	char n1[PATH_LEN], n2[PATH_LEN];

	/* Open the file */
	if (NULL == (trn_fd = fopen (trn_file, "r"))) {
		fprintf (stderr, "%s: can't open %s, tran table not used\n",
			progname, trn_file);
		exit(-1);	/* Return failure errorcode to caller */
	}

	/* Read through the file */
	while (2 == fscanf (trn_fd, "%s %s", n1, n2)) {
		/* Get space for new list entry */

		if (NULL == (tp = (struct trn_ent *)
				malloc (sizeof (struct trn_ent)))) {
			OutOfMemory();
		}
		if (NULL == (tp->old = (char *) malloc (strlen(n1)+1))) {
			OutOfMemory();
		}
		if (NULL == (tp->new = (char *) malloc (strlen(n2)+1))) {
			OutOfMemory();
		}

		/* Fill in new entry */
		strcpy (tp->old, n1);
		strcpy (tp->new, n2);

		/* Add to list */
		tp->next = trn_list;
		trn_list = tp;
	}

	fclose (trn_fd);
	return (0);
}
