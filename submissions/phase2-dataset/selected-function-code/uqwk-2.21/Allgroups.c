FILE *pfd;
{
	struct act_ent *ap;

	if (ReadActive() < 0) {
		fprintf (pfd, "Sorry, no newsgroups are available.\n");
		return (0);
	}

	fprintf (pfd, "List of available newsgroups:\n\n");

	ap = act_list;
	while (ap != NULL) {
		fprintf (pfd, "    %s (%d articles)\n",
			ap->name, ap->hi - ap->lo);
		ap = ap->next;
	}
	return (1);
}
