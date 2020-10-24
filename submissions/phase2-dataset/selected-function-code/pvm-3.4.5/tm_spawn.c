int
tm_spawn(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	char *where = 0;				/* location from req */
	struct waitc *wp;				/* 'seed' waitc */
	struct waitc_spawn *wxp = 0;	/* new task parameters */
	struct htab *htp;				/* set of usable hosts */
	struct hostd *hp;
	int hh;
	int i;
	char *wd = 0;
	char *wdenv = 0;
	char *ptr;
	int sz;

	/*
	* unpack spawn command from task
	*/

	wxp = TALLOC(1, struct waitc_spawn, "waix");
	BZERO((char*)wxp, sizeof(struct waitc_spawn));

	if (upkstralloc(mp, &wxp->w_file)
	|| upkint(mp, &wxp->w_flags)
	|| upkstralloc(mp, &where)
	|| upkint(mp, &wxp->w_veclen)
	|| upkint(mp, &wxp->w_argc))
		goto bad;

	if (wxp->w_veclen < 1)
		goto bad;

	wxp->w_argv = TALLOC(wxp->w_argc + 1, char*, "argv");
	BZERO((char*)wxp->w_argv, (wxp->w_argc + 1) * sizeof(char*));
	for (i = 0; i < wxp->w_argc; i++)
		if (upkstralloc(mp, &wxp->w_argv[i]))
			goto bad;

	if (upkuint(mp, &wxp->w_outtid)
	|| upkuint(mp, &wxp->w_outctx)
	|| upkuint(mp, &wxp->w_outtag)
	|| upkuint(mp, &wxp->w_trctid)
	|| upkuint(mp, &wxp->w_trcctx)
	|| upkuint(mp, &wxp->w_trctag))
		goto bad;

	/*
	* extract any working directory string from "where"
	*/

	ptr = where;
	while ( *ptr != ':' && *ptr != '\0' )
		ptr++;
	if ( *ptr == ':' ) {
		*ptr++ = '\0';    /* close off actual "where" string */
		wd = ptr;         /* save ptr to working directory */
	}

	/*
	* unpack environment from task
	*/

	if (upkuint(mp, &wxp->w_nenv))
		goto bad;
	sz = wxp->w_nenv + 1 + (wd ? 1 : 0);
	wxp->w_env = TALLOC(sz, char*, "env");
	BZERO((char*)wxp->w_env, sz * sizeof(char*));
	for (i = 0; i < wxp->w_nenv; i++)
		if (upkstralloc(mp, &wxp->w_env[i]))
			goto bad;

	/*
	* add extra env string for working directory
	*/

	if ( wd ) {
		sz = strlen(wd) + strlen("PVMSPAWNWD=") + 1;
		wdenv = TALLOC(sz, char, "wdenv");
		sprintf( wdenv, "PVMSPAWNWD=%s", wd );
		wxp->w_env[ (wxp->w_nenv)++ ] = STRALLOC( wdenv );
	}

	/*
	* make host set containing hosts (matching where option)
	*/

	if ((wxp->w_flags & (PvmTaskHost|PvmTaskArch)) && !where)
		goto bad;

	htp = ht_new(1);

	if (wxp->w_flags & PvmTaskHost) {			/* given host */
		if (hp = nametohost(hosts, where))
			ht_insert(htp, hp);

	} else {
		if (wxp->w_flags & PvmTaskArch) {		/* given arch */
			for (hh = hosts->ht_last; hh > 0; hh--)
				if ((hp = hosts->ht_hosts[hh])
				&& !strcmp(where, hp->hd_arch))
					ht_insert(htp, hp);

#ifdef IMA_BEOLIN
		/* local pvmd for now */
		} else if (wxp->w_flags & PvmMppFront) {
			hp = hosts->ht_hosts[hosts->ht_local];
			ht_insert(htp, hp);
#endif

		} else {						/* anywhere */
			ht_merge(htp, hosts);
		}
	}

	if (wxp->w_flags & PvmHostCompl) {
		for (hh = hosts->ht_last; hh > 0; hh--) {
			if (hh <= htp->ht_last && (hp = htp->ht_hosts[hh]))
				ht_delete(htp, hp);
			else
				if (hp = hosts->ht_hosts[hh])
					ht_insert(htp, hp);
		}
	}

	if (pvmdebmask & PDMTASK) {
		pvmlogerror("tm_spawn() host set:\n");
		ht_dump(htp);
	}

	if ( !(wxp->w_flags & PvmNoSpawnParent) ) {
		wxp->w_ptid = tp->t_tid;
	}
	else
		wxp->w_ptid = PvmParentNotSet;	/* indicate task unset parent */

	/*
	* assign each task to a host
	*/

	wxp->w_ht = htp;
	wxp->w_vec = TALLOC(wxp->w_veclen, int, "vec");
	BZERO((char*)wxp->w_vec, wxp->w_veclen * sizeof(int));
	wxp->w_togo = wxp->w_veclen;

	wp = wait_new(WT_SPAWN);
	wp->wa_tid = tp->t_tid;
	wp->wa_spec = (void*)wxp;
	wxp = 0;
	wp->wa_mesg = replymessage(mp);

	assign_tasks(wp);

	/* if already done, reply to task */

	if (wp->wa_peer == wp) {
		assign_tasks(wp);
	}

	wait_delete(wp);
	goto cleanup;

bad:
	pvmlogprintf("tm_spawn() from t%x bad msg format\n", mp->m_src);

cleanup:
	if (where)
		PVM_FREE(where);
	if (wxp)
		free_wait_spawn(wxp);
	return 0;
}
