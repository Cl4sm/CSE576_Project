/*
 *  Select articles from a summary file
 */
{
	char group[PATH_LEN], news_path[PATH_LEN];
#ifndef NNTP
	char *p;
#endif
	int artnum;
	struct act_ent *ap;
	struct nrc_ent *np = NULL, *tnp;
	struct conf_ent *cp = NULL;

	/* Open selection file */
	if (NULL == (sel_fd = fopen (sel_file, "r"))) {
		fprintf (stderr, "%s: can't open %s\n", progname, sel_file);
		return -1;
	}

	/* Show no group yet */
	group[0] = 0;

	/* Read through lines */
	while (NULL != Fgets (buf, BUF_LEN, sel_fd)) {
		/* Check if too many blocks already */
		if ( (blk_cnt >= max_blks) && (max_blks > 0) ) break;

		/* Ignore empty lines */
		if (0 == strlen (buf)) continue;

		/* First character determines type of line */
		switch (buf[0]) {
		case '*':	/* New group */

			/* Close old index files if any */
			if (group[0] != 0) {
				if (!slnp_mode && !zip_mode && !sum_mode)
							NdxClose (ndx_fd);
				if (slnp_mode) MsgClose (msg_fd);
			}

			/* New newsgroup */
			if (1 != sscanf (buf, "%*s %s", group)) {
				group[0] = 0;
				break;
			}

			/* Find it in newsrc, bail if not there */
			np = NULL;
			for (tnp=nrc_list; tnp!=NULL; tnp=tnp->next) {
				if (!strcmp (tnp->name, group)) np = tnp;
			}
			if (np == NULL) {
				group[0] = 0;
				break;
			}

			printf ("%s: %s\n", progname, group);

			/* Look up in active file */
			ap = FindActive (group);

			/* Do nothing if not in active file */
			if (ap == NULL) {
				/* Show no valid group */
				group[0] = 0;
				break;
			}

			/* Make new conference */
			cp = NewConference (group, np->conf);

			/* No ZipNews index yet */
			if (zip_mode) zip_flag = 0;
#ifdef NNTP
	#ifdef NTP_FULL_ACTIVE
			/* Select group from NNTP server */
			group_nntp (group);
	#endif
#else
			/* Make news path */
			sprintf (news_path, "%s/%s/", news_dir, group);
			for (p=(&news_path[0]); *p; p++)
				if (*p == '.') *p = '/';
#endif
			/* Fix up the subscription list */
			np->sub = FixSub (np->sub, ap->lo, ap->hi);

			break;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			/* Wants article; skip if no group */
			if (group[0] != 0) {
				/* Get article number */
				sscanf (buf, "%d", &artnum);

				/* Process it */
				if( DoArticle(news_path, artnum, np, cp)==-2)
					return -2;

				/* Mark as read */
				np->sub = MarkRead (artnum, np->sub);
			}
			break;

		default:
			break;
		}
	}

	fclose (sel_fd);
	return (1);
}
