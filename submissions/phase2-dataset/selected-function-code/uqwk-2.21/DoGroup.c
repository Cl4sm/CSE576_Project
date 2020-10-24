struct nrc_ent *np;
/*
 *  Process given group
 */
{
	struct act_ent *ap;
	char news_path[PATH_LEN];
	int i, n;
	struct conf_ent *cp;
#ifdef SCORES
        int nArticles = 0;
        int nKills = 0;
#endif

	printf ("%s", np->name);
#ifdef SCORES
        fflush(stdout);
#endif

	/* Lookup group in active file */
	ap = FindActive (np->name);
	/* If the group doesn't exist (that is, doesn't appear in
	   the active file), do nothing else */
	if (ap == NULL) {
	        printf (": no such group\n");
		return 0;
	}


	/* Make a new conference with this name */
	cp = NewConference (np->name, np->conf);

	/* Remember no ZipNews index entry yet */
	if (zip_mode) zip_flag = 0;

#ifdef NNTP
	#ifdef NNTP_FULL_ACTIVE
	/* select group name from news server */
	group_nntp(np->name);
	#endif
#else
	/* Construct path name for articles in this group */
	strcpy (news_path, news_dir);
	strcat (news_path, "/");
	strcat (news_path, np->name);
	strcat (news_path, "/");
	n = strlen (news_path);
	for (i=0; i<n; i++) if (news_path[i] == '.') news_path[i] = '/';
#endif

	/* Fix up the subscription list */
	np->sub = FixSub (np->sub, ap->lo, ap->hi);

	/* Look through unread articles */
	for (i=ap->lo; i<=ap->hi; i++) {
		/* Check max block count */
		if ( (blk_cnt >= max_blks) && (max_blks > 0) ) {
			if (!slnp_mode && !zip_mode && !sum_mode)
							NdxClose (ndx_fd);
			if (slnp_mode) MsgClose (msg_fd);
#ifdef SCORES
			if (do_kills)
       printf(" %d articles obtained, %d killed", nArticles, nKills);
#endif
			printf("\n");
			return (-1);
		}

		/* Process this article */
		if (!IsRead (i, np->sub)) {
			/* Mark as read */
			np->sub = MarkRead (i, np->sub);

/* Process the article */
#ifdef SCORES
			n=DoArticle(news_path, i, np, cp);
			switch(n){
				case 1:  ++nArticles; break;
				case -1: ++nKills; break;
				case 0:  continue;
				default: return(n);
			}
#else  /* ndef SCORES */
			if((n=DoArticle(news_path, i, np, cp))!=1)
				return(n);
#endif /* SCORES */
		}
	}
	if (!slnp_mode && !zip_mode && !sum_mode) NdxClose (ndx_fd);
	if (slnp_mode) MsgClose (msg_fd);

#ifdef SCORES
	if (do_kills)
       printf(" %d articles obtained, %d killed", nArticles, nKills);
#endif /* SCORES */
	printf("\n");

	return (1);
}
