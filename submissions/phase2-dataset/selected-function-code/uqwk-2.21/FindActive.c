char *c;
/*
 *  Look for group's active entry given group name
 */
{
	struct act_ent *ap;
#ifdef NNTP
	int dummy;
#endif

#if defined(NNTP) && !defined(NNTP_FULL_ACTIVE)
	/* Ask server about group */
	sprintf(buf,"GROUP %s", c);
	put_server(buf);
	nntp_get(buf, sizeof(buf));
	if (*buf != CHAR_OK) {		/* and then see if that's ok */
		if(atoi(buf) == ERR_NOGROUP) {
			fprintf(stderr, "%s: %s does not exist (server said: %s)\n",
			        progname, c, buf);
		} else {
			fprintf(stderr, "%s: `GROUP %s' server response: %s\n",
			        progname, c, buf);
		}
	} else {
		/* Get new act entry */
		ap = (struct act_ent *) malloc (sizeof (struct act_ent));
		if (ap == NULL) OutOfMemory();

		/* Parse name, message numbers; `buf' contains a server response */
		sscanf (buf, "%d %d %d %d", &dummy, &dummy, &ap->lo, &ap->hi);
		if((ap->name = (char *) malloc (1+strlen(c))) == NULL) OutOfMemory();;
		strcpy (ap->name, c);

		/* Add to list */
/*	No need to -- better not to cache, and always do a fresh GROUP
		ap->next = act_list;
		act_list = ap;						*/

		return(ap);
	}
#else	/* Not in NNTP mode, or in NNTP_FULL_ACTIVE mode */
	ap = act_list;
	while (NULL != ap) {
		if (!strcmp (c, ap->name)) return (ap);
		ap = ap->next;
	}
#endif

	return (NULL);
}
