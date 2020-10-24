mpprecv(tid, tag, cp, len, rtid, rtag, rlen)
	int tid;
	int tag;
	void *cp;
	int len;
	int *rtid;
	int *rtag;
	int *rlen;
{
#if defined(IMA_PGON) || defined(IMA_CM5)
	long info[8];
#endif
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	MPI_Status info;
	int mpiflag = 0;
	int mpisiz;
#endif
	long node;
	struct pmsg *up;
	int cc;

	node = (tid == -1) ? MPPANYNODE : tid & pvmtidnmask;
#if defined(IMA_CM5) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
	for (up = precvlist->m_link; up != precvlist; up = up->m_link)
		/* message picked up by psend */
		if ((tag == -1 || tag == up->m_tag) 
		&& (tid == -1 || node == up->m_src)) {
			struct frag *fp;

			fp = up->m_frag->fr_rlink;
			if (fp->fr_max < len)
				len = fp->fr_max;
			BCOPY(fp->fr_dat, cp, len);
			if (rlen)
				*rlen = len;
			if (rtid)
				*rtid = up->m_src + (pvmmytid & ~pvmtidnmask);
			if (rtag)
				*rtag = up->m_tag;
			LISTDELETE(up, m_link, m_rlink);
			if (pvmdebmask & PDMPACKET) {
				sprintf(pvmtxt, "pvm_precv() task %x len %d tag %d\n",
					up->m_src + (pvmmytid & ~pvmtidnmask), 
					rlen ? *rlen : fp->fr_max, rtag ? *rtag : up->m_tag);
					pvmlogerror(pvmtxt);
			}
			umbuf_free(up);
			return 1;
		}
#endif
	
	/* 
	 * Msg routed from pvmd has type PMTPACK, so it may have already
	 * been picked up by the last pvm_recv. If so, we call pvm_recv
	 * to process it. Note we only need consider the case tid = -1,
	 * because msg psent by another node cannot have type PMTPACK.
	 */
	if (tid == -1) {
		for (up = pvmrxlist->m_link; up != pvmrxlist; up = up->m_link)
			if (tag == -1 || tag == up->m_tag)
				return 0;			/* go on to pvm_recv */
		if (tag != -1)
			/* wait for msg with tag or PMTPACK */
			while (!MSGPROBE(MPPANYNODE, tag, info))
				if (MSGPROBE(pvmhostnode, PMTPACK, info)) {
					if ((cc = mroute(0, 0, 0, (struct timeval *)0)) < 0)
						return cc;
					for (up = pvmrxlist->m_link; up != pvmrxlist; 
					up = up->m_link)
						if (tag == up->m_tag)
							return 0;			/* go on to pvm_recv */
				}
	}

	/* 1) from another node: 1a) tid = -1; 1b) tid given; 2) tag = -1 */
#ifndef IMA_PGON
	if (tag == -1)
		tag = MPPANYTAG;
#endif
	if ((mpierrcode = 
	PVMCRECV(node, tag, (char*)cp, len, pvmmyptype, info)) 
#if defined(IMA_PGON) || defined(IMA_CM5)
	&& (mpierrcode < 0)
#endif
	) {
		pvmlogperror("precv() PVMCRECV");
		return PvmSysErr;
	} 
	if ((node = MSGSENDER(info)) == pvmhostnode)
		/* got null msg; go on to pvm_recv */
		return 0;
			
	if (rlen)
		*rlen = MSGSIZE(info);
	if (rtid)
		*rtid = node + (pvmmytid & ~pvmtidnmask);
	if (rtag)
		*rtag = MSGTAG(info);
	if (pvmdebmask & PDMPACKET) {
		sprintf(pvmtxt, "pvm_precv() task %x len %d tag %d\n",
			node + (pvmmytid & ~pvmtidnmask), rlen ? *rlen : MSGSIZE(info), 
			rtag ? *rtag : MSGTAG(info));
		pvmlogerror(pvmtxt);
	}
	return 1;
}
