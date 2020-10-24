int Xref (s)
char *s;
/*
 *  Process an Xref line.
 */
{
	char *c, *p, name[PATH_LEN];
	int num;
	struct nrc_ent *np;

	/* Skip the first two fields (Xref: and host) */
	c = strtok (s, " \t");
	if (c == NULL)
		return 0;
	c = strtok (NULL, " \t");
	if (c == NULL)
		return 0;

	/* Look through the rest of the fields */
	c = strtok (NULL, " \t");
	while (c != NULL) {
		/* Change : to space */
		for (p=c; *p; p++) if (*p == ':') *p = ' ';

		/* Parse xref entry */
		if (2 == sscanf (c, "%s %d", name, &num)) {
			/* Find nrc entry for this group */
			for (np=nrc_list; np!=NULL; np=np->next) {
				/* Match? */
				if (!strcmp (np->name, name)) {
					/* Mark as read */
					np->sub = MarkRead (num, np->sub);
					break;
				}
			}
		}

		/* Next xref entry */
		c = strtok (NULL, " \t");
	}

	return 0;
}
