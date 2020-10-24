int ReadNewsrc()
/*
 *  Read the .newsrc file
 */
{
	char group_name[PATH_LEN];
	struct nrc_ent *np, *lnp;
	int i, n, c;
	char *buf;

	/* lnp points to last entry */
	lnp = NULL;

	/* Don't bother if we've alread read it */
	if (nrc_list != NULL) return (1);

	/* Allocate a buffer for reading */
	if((buf = (char *) malloc(BUF_LEN)) == NULL)
		OutOfMemory();

	/* Open it */
	if (NULL == (nrc_fd = fopen (nrc_file, "r"))) {
		fprintf (stderr, "%s: can't open %s\n", progname, nrc_file);
		return(-1);
	}

	/* Read through */
	while (NULL != Fgets (buf, BUF_LEN, nrc_fd)) {
		/* Allocate a new nrc entry */
		np = (struct nrc_ent *) malloc (sizeof (struct nrc_ent));
		if (np == NULL) OutOfMemory();

		if (waf_mode) {
			/* Waffle is easy */
			sscanf (buf, "%s %d", group_name, &n);
			np->subscribed = 1;

			/* Allocate sub entry */
			if (NULL == (np->sub = (struct sub_ent *) malloc
				(sizeof (struct sub_ent)))) OutOfMemory();

			np->sub->lo = 0;
			np->sub->hi = n;
			np->sub->next = NULL;
		} else { /* Regular Unix-style .newsrc */
			/* Assume not subscribed */
			np->subscribed = 0;
			/* Avoid bus error on missing ! or : */
			np->sub = 0;

			/* Parse group name */
			n = strlen (buf);
			for (i=0; i<n; i++) {
				/* Some .newsrc's don't have a space after the
			   newsgroup name, so kludge it like this */
				if (buf[i] == ':') {
					np->subscribed = 1;
					buf[i] = ' ';

					/* Parse subscription list */
					np->sub = SubList (&buf[i+1]);
				}
				if (buf[i] == '!') {
					np->subscribed = 0;
					buf[i] = ' ';

					/* Parse subscription list */
					np->sub = SubList (&buf[i+1]);
				}
			}
			sscanf (buf, "%s", group_name);
		}

		n = strlen (group_name);

		np->name = (char *) malloc (n+1);
		if (np->name == NULL) OutOfMemory();
		strcpy (np->name, group_name);
		np->next = NULL;

		/* Add to nrc list */
		if (lnp == NULL) {
			/* First one */
			nrc_list = np;
		} else {
			/* Add to end */
			lnp->next = np;
		}
		lnp = np;
	}

	/* Walk through the nrc list, assign conference numbers */
	np = nrc_list;
	c = 0;
	while (np != NULL) {
		if (np->subscribed) {
			np->conf = c;
			c++;
		} else {
			np->conf = (-1);
		}
		np = np->next;
	}

	free(buf);
	fclose (nrc_fd);
	return (1);
}
