/*
 *  Collect unread news into a packet
 */
{
#if defined(NNTP_FULL_ACTIVE)
	char active_name[MAXFILENAME];
#endif
	struct nrc_ent *np;
	struct ng_ent *ngp;
	int	ret;
#ifdef SCORES
	int	has_score = 0;
#endif

	/* Open ZipNews files */
	if (zip_mode) {
		if ((ret=OpenZipFiles())<=0) {
			fprintf(stderr,"%s: can't open zipfiles\n", progname);
			return(ret);
		}
	}

	/* Read .newsrc file */
	if ((ret=ReadNewsrc()) < 0) {
		fprintf(stderr,"%s: can't read newsrc file\n", progname);
		return(ret);
	}

#if !defined(NNTP) || defined(NNTP_FULL_ACTIVE)
	/* And active file */
	if ((ret=ReadActive()) < 0) {
		fprintf(stderr,"%s: can't read active file\n", progname);
		return(ret);
	}
#endif

	/* Handle selection mode */
	if (sel_mode) {
		if((ret=DoSelect())!=1) return(ret);
#if defined(NNTP_FULL_ACTIVE)
		sprintf(active_name,"/tmp/rrnact.%d",getpid());
		unlink(active_name);
#endif
		/* ZipNews "join" file */
		if (zip_mode) {
			if((ret=WriteJoin())!=1) return(ret);
			fclose (nws_fd);
			fclose (idx_fd);
		}
		return (0);
	}

#ifdef SCORES
        if (do_kills) {
	   if(read_score_file (GLOBAL_SCORE_FILE))
		   printf("(global scoring)\n");
	   else
		   printf("(no global scoring)\n");
	}
#endif

	/* Use .newsrc or desire newsgroup file? */
	if (ng_list == NULL) {
		/*
		 *  No desired ng file, use newsrc
		 */
		np = nrc_list;
		while (np != NULL) {
			/* Check if too many blocks already */
			if ( (blk_cnt >= max_blks) && (max_blks > 0) ) {
#ifdef NNTP_FULL_ACTIVE
				sprintf(active_name,"/tmp/rrnact.%d",getpid());
				unlink(active_name);
#endif
				/* ZipNews "join" file */
				if (zip_mode) {
					if((ret=WriteJoin())!=1) return(ret);
					fclose (nws_fd);
					fclose (idx_fd);
				}
				return (0);
			}

			if (np->subscribed) {
#ifdef SCORES
			        if (do_kills) {
				    group_kill_thresh = kill_thresh;
				    has_score = read_score_file (np->name);
				}
#endif
				/* Do this group */
				ret=DoGroup(np);
#ifdef SCORES
			        if (do_kills && has_score) free_group_score();
#endif
				switch(ret) {
					case -1:
						fprintf(stderr,"%s: can't entirely do group %s\n", progname, np->name);
						return(0);
						break;
					case 0:
						break;
					case 1:
						break;
					default:
						fprintf(stderr,"%s: can't do group %s\n", progname, np->name);
						return(ret);
						break;
				}

			}
			np = np->next;
		}
#ifdef NNTP_FULL_ACTIVE
		sprintf(active_name,"/tmp/rrnact.%d",getpid());
		unlink(active_name);
#endif
		/* ZipNews "join" file */
		if (zip_mode) {
			if((ret=WriteJoin())!=1) return(ret);
			fclose (nws_fd);
			fclose (idx_fd);
		}
	} else {
		/*
		 *  Read desired ng file for groups
		 */
		for (ngp=ng_list; ngp!=NULL; ngp=ngp->next) {
			/* Check if too many blocks already */
			if ( (blk_cnt >= max_blks) && (max_blks > 0) ) {
#ifdef NNTP_FULL_ACTIVE
				sprintf(active_name,"/tmp/rrnact.%d",getpid());
				unlink(active_name);
#endif
				/* ZipNews "join" file */
				if (zip_mode) {
					if((ret=WriteJoin())!=1) return(ret);
					fclose (nws_fd);
					fclose (idx_fd);
				}
				return (0);
			}

			/* Find .newsrc entry, book if none */
			for (np=nrc_list; np!=NULL; np=np->next) {
				if (!strcmp (ngp->name, np->name)) break;
			}

			if (np == NULL) {
				fprintf (stderr, "%s: %s not in .newsrc\n",
						progname, ngp->name);
			} else {
#ifdef SCORES
			        if (do_kills) read_score_file (np->name);
#endif
				/* Do this group */
				ret=DoGroup(np);
#ifdef SCORES
			        if (do_kills) free_group_score();
#endif
				switch(ret !=1) {
					case -1:
						fprintf(stderr,"%s: can't entirely do group %s\n", progname, np->name);
						return(0);
						break;
					case 0:
						break;
					case 1:
						break;
					default:
						fprintf(stderr,"%s: can't do group %s\n", progname, np->name);
						return(ret);
						break;
				}
			}
		}
#ifdef NNTP_FULL_ACTIVE
		sprintf(active_name,"/tmp/rrnact.%d",getpid());
		unlink(active_name);
#endif
		/* ZipNews "join" file */
		if (zip_mode) {
			if((ret=WriteJoin())!=1) return(ret);
			fclose (nws_fd);
			fclose (idx_fd);
		}
	}

	return (1);
}
