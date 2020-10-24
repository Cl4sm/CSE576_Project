FILE *pfd;
{
	struct act_ent *ap;
	struct nrc_ent *np;
	struct sub_ent *sp, *tsp;
	char group[PATH_LEN];

	/* Extract group name */
	if (1 != sscanf (buf, "%*s %s", group)) {
		fprintf (pfd, "Usage: CATCHUP newsgroup\n");
		return (0);
	}

#if !defined(NNTP) || defined(NNTP_FULL_ACTIVE)
	/* We will need active file and .newsrc */
	if ((ReadActive() < 0) || (ReadNewsrc() < 0)) {
#else
	if (ReadNewsrc() < 0) {
#endif
		fprintf (pfd, "Sorry, couldn't read system files.\n");
		return (-1);
	}

	/* Not subscribed? */
	np = nrc_list;
	while (np != NULL) {
		if (!strcmp (group, np->name)) {
			if (np->subscribed) {
				break;
			} else {
				fprintf (pfd,
					"You are not subscribed to %s.\n",
					group);
				return (0);
			}
		}
		np = np->next;
	}

	if (np == NULL) {
		fprintf (pfd, "You are not subscribed to %s.\n", group);
		return (0);
	}

	/* Find group in active file */
	if (NULL == (ap = FindActive (group))) {
		fprintf (pfd, "No such newsgroup: %s\n", group);
		return (0);
	}

	/* Free subscription list */
	sp = np->sub;
	while (sp != NULL) {
		tsp = sp->next;
		free (sp);
		sp = tsp;
	}

	/* Okay already, add to .newsrc */
	np = (struct nrc_ent *) malloc (sizeof (struct nrc_ent));
	if (np == NULL) OutOfMemory();
	np->name = (char *) malloc (1+strlen(group));
	if (np->name == NULL) OutOfMemory();
	strcpy (np->name, group);
	np->subscribed = 1;

	/* Make subscription list - everything is read */
	if (NULL == (np->sub = (struct sub_ent *) malloc
			(sizeof (struct sub_ent)))) OutOfMemory();
	np->sub->lo = 1;
	np->sub->hi = ap->hi;
	np->sub->next = NULL;

	np->next = nrc_list;
	nrc_list = np;

	WriteNewsrc();
	fprintf (pfd, "Okay, you are now caught up in %s.\n", group);

	return (1);
}
