int WriteJoin ()
/*
 *  Write the ZipNews join file
 */
{
	FILE *fd;
	char fn[PATH_LEN];
	struct nrc_ent *np;

	/* Open join file */
	sprintf (fn, "%s/%s.jn", home_dir, user_name);
	if (NULL == (fd = fopen (fn, "w"))) {
		fprintf (stderr, "%s: can't open %s\n", progname, fn);
		return -1;
	}

	/* Walk through the newsrc */
	np = nrc_list;
	while (np != NULL) {
		if (np->subscribed) {
			if (np->sub == NULL) {
				if((fprintf(fd, "%s 0\r\n", np->name) < 0)
				&& ferror(fd) )
					return -1;
			} else {
				if((fprintf (fd, "%s %d\r\n", np->name, np->sub->hi) < 0)
				&& ferror(fd) )
					return -1;
			}
		}
		np = np->next;
	}

	fclose (fd);
	return 1;
}
