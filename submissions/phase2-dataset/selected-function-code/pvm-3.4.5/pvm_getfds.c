pvm_getfds(fds)		/* XXX this function kinda sucks */
	int **fds;			/* fd list return */
{
	static int *fdlist = 0;
	static int fdlen = 0;
	int cc;
	int nfds;
	struct ttpcb *pcbp;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_GETFDS,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	if (!(cc = BEATASK)) {
		nfds = 1;
		for (pcbp = ttlist->tt_link; pcbp != ttlist; pcbp = pcbp->tt_link)
			if (pcbp->tt_state == TTOPEN || pcbp->tt_state == TTGRNWAIT)
				nfds++;

		if (fdlen < nfds) {
			fdlen = (nfds * 3) / 2 + 1;
			if (fdlist)
				fdlist = TREALLOC(fdlist, fdlen, int);
			else
				fdlist = TALLOC(fdlen, int, "fdlist");
		}

		fdlist[0] = topvmd->tt_fd;
		nfds = 1;
		for (pcbp = ttlist->tt_link; pcbp != ttlist; pcbp = pcbp->tt_link)
			if (pcbp->tt_state == TTOPEN || pcbp->tt_state == TTGRNWAIT)
				fdlist[nfds++] = pcbp->tt_fd;
		*fds = fdlist;
		cc = nfds;
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_GETFDS,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			if (cc > 0) {
				TEV_PACK_INT( TEV_DID_FDS, TEV_DATA_ARRAY,
					(int *)fdlist, nfds, 1 );
			}
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	return (cc < 0 ? lpvmerr("pvm_getfds", cc) : cc);
}
