peer_conn(tid, new_connection)
	int tid;
	int *new_connection;
{
	struct peer *pp;
	char *p;

	if (new_connection)
		*new_connection = 0;
	for (pp = peers->p_link; pp != peers; pp = pp->p_link) {
		if (pp->p_tid == tid) {		/* already connected */
			if (pp->p_exited || !pp->p_buf) {
				if (pvmdebmask & PDMMEM) {
					sprintf(pvmtxt, "peer_conn() t%x has exited?\n", tid);
				 	pvmlogerror(pvmtxt);
				 }
				return ((struct peer *)-1L);
				}
			else {
				if (pvmdebmask & PDMMEM) {
					sprintf(pvmtxt, "peer_conn() to t%x got pp addr %lx\n", 
							tid, pp);
				 	pvmlogerror(pvmtxt);
				 }
				return pp;

				}
		}
	/*
	* test p_buf in case we're in the process of destroying this peer struct
	*/

#if 0
	/* XXX disconnect from inactive peers */
		if (pp2->p_tid == -1) {		/* used by pvmd */
			/* peer_detach(pp2); */
			shmdt(pp2->p_buf);
			LISTDELETE(pp2, p_link, p_rlink);
			PVM_FREE(pp2);
		}
#endif
	}

	if (pp == peers && tid == pvmmytid) {
		if (!(pp = TALLOC(1, struct peer, "peer"))) {
			pvmlogerror("peer_conn() can't get memory\n");
			return (struct peer *)0;
		}
		BZERO((char*)pp, sizeof(struct peer));
		pp->p_buf = pvminbox;
		pp->p_shmid = myshmbufid;
		pp->p_tid = pvmmytid;
		pp->p_semid = -1;
		LISTPUTAFTER(peers, pp, p_link, p_rlink);

	} else if (pp == peers && tid) {
		int bufid, ntids, i, key;

		/* PAGELOCK(pvminfo); */
	/* XXX the first few don't have to search the whole empty table. hmm. */
		ntids = min(maxpidtid, ((struct shmpghdr *)infopage)->pg_ref);
		for (i = 0; i < ntids; i++)
			if (pidtids[i].pt_tid == tid && pidtids[i].pt_stat != ST_EXIT) {
				if (pidtids[i].pt_stat == ST_NOTREADY)
					return (struct peer *)0;
				if (pidtids[i].pt_stat == ST_SOCKET)
					return (struct peer *)-1L;
				/* must be ST_SHMEM or ST_FINISH */
				break;
			}
		if (i == ntids) 
			{
			if (pvmdebmask & PDMMEM) {
				sprintf(pvmtxt, "peer_conn() could not find t%x in daemon list\n", tid);
			 pvmlogerror(pvmtxt);
			 }
			return (struct peer *)-1L;
		/* PAGEUNLOCK(pvminfo); */
		}

		key = pidtids[i].pt_key;
		if ((bufid = shmget((key_t)key, shmbufsiz, 0)) == -1) {
			sprintf(pvmtxt, "peer_conn() shmget t%x key 0x%x", tid, key);
			pvmlogperror(pvmtxt);
			return (struct peer *)0;
		}
		while ((p = (char *)shmat(bufid, 0, 0)) == (char *)-1L) {
			sprintf(pvmtxt, "peer_conn() shmat to t%x using bufid %d\n", 
					tid, bufid);
			pvmlogperror(pvmtxt);
			if (errno == EMFILE && (pp = peers->p_rlink) != peers) {
				shmdt(pp->p_buf);
				LISTDELETE(pp, p_link, p_rlink);
				PVM_FREE(pp);
				continue;
			}
			return (struct peer *)0;
		}

#ifdef IMA_CSPP
		_touch(p);
#endif

		if (!(pp = TALLOC(1, struct peer, "peer"))) {
			pvmlogerror("peer_conn() can't get memory\n");
			return (struct peer *)0;
		}
		BZERO((char*)pp, sizeof(struct peer));
		pp->p_key = key;
		pp->p_tid = tid;
		pp->p_buf = p;
		pp->p_shmid = bufid;
		pp->p_semid = -1;
#ifdef IMA_CSPP
		pp->p_node = pidtids[i].pt_node;
#endif
		pp->p_exited = 0;
		LISTPUTAFTER(peers, pp, p_link, p_rlink);
		if (new_connection)
			*new_connection = 1;
	}

	return pp;
}
