long offset;
{
	struct mail_ent *mp;

	/* Get space for new mail list entry */
	if (NULL==(mp=(struct mail_ent *) malloc(sizeof(struct mail_ent)))) {
		fprintf (stderr, "%s: out of memory\n", progname);
		exit(-1);
	}

	/* Fill in offset */
	mp->begin = offset;

	if (last_mp == NULL) {
		/* This is first message */
		mail_list = mp;
	} else {
		/* Add to end of list */
		last_mp->next = mp;
		last_mp->end = offset;
	}

	mp->next = NULL;
	last_mp = mp;
}
