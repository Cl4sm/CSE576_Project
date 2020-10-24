/*
 *  Read the desired newsgroups file
 */
{
	FILE *ng_fd;
	struct ng_ent *np, *last_np = NULL;
	char n[PATH_LEN];

	/* Open the file */
	if (NULL == (ng_fd = fopen (ng_file, "r"))) {
		fprintf (stderr, "%s: can't open %s, ng file not used\n",
			progname, ng_file);
		exit(-1);	/* Return failure errorcode to caller */
	}

	/* Read through the file */
	while (1 == fscanf (ng_fd, "%s", n)) {
		/* Get space for new list entry */

		if (NULL == (np = (struct ng_ent *)
				malloc (sizeof (struct ng_ent)))) {
			OutOfMemory();
		}
		if (NULL == (np->name = (char *) malloc (strlen(n)+1))) {
			OutOfMemory();
		}

		/* Fill in new entry */
		strcpy (np->name, n);

		/* Add to list */
		if (ng_list == NULL) {
			/* First one */
			ng_list = np;
		} else {
			/* Add to end */
			last_np->next = np;
		}
		np->next = NULL;
		last_np = np;
	}

	fclose (ng_fd);
	return (0);
}
