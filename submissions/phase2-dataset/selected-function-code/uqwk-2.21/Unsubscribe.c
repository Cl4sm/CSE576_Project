int Unsubscribe (pfd)
FILE *pfd;
{
	struct nrc_ent *np;
	char group[PATH_LEN];

	/* Parse group name */
	if (1 != sscanf (buf, "%*s %s", group)) {
		fprintf (pfd, "Usage: UNSUBSCRIBE newsgroup\n");
		return 0;
	}

	/* Check for ALL */
	if ( (!strcmp (group, "ALL")) || (!strcmp (group, "all")) ) {
		nrc_list = NULL;
		WriteNewsrc();
		fprintf (pfd,
		  "Okay, you are now unsubscribed from all newsgroups.\n");
		return 0;
	}

	/* We need the .newsrc file */
	if (ReadNewsrc() < 0) {
		fprintf (pfd, "Sorry, couldn't read .newsrc\n");
		return -1;
	}

	/* Look for group in newsrc */
	np = nrc_list;
	while (np != NULL) {
		if (!strcmp (group, np->name)) break;
		np = np->next;
	}

	if (np == NULL) {
		fprintf (pfd, "You are not currently subscribed to %s.\n",
		         group);
		return 0;
	}

	np->subscribed = 0;

	WriteNewsrc();
	fprintf (pfd, "Okay, you are unsubscribed from %s.\n", group);

	return 1;
}
