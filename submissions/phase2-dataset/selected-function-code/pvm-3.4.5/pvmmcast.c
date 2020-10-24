pvmmcast(mid, tids, count, tag)
	int mid;
	int *tids;
	int count;
	int tag;
{
	static struct timeval ztv = { 0, 0 };

	int *dst;
	int i, j;
	int cc = 0;
	struct ttpcb *pcbp;
	int sbf;
	struct pmsg *mp;

	/*
	* make sorted list of destinations
	*/
	dst = TALLOC(count, int, "mcal");
	BCOPY(tids, dst, count * sizeof(int));
	qsort((char*)dst, count, sizeof(int), int_compare);

	/* 	pvmsbuf->m_ctx = pvmmyctx; */
	/* we are sending mesg mid which may not the same as pvmsbuf->m_mid */
	mp = midtobuf(mid);
	mp->m_ctx = pvmmyctx;

	/*
	* remove duplicates
	*/
	j = 0;
	for (i = 1; i < count; i++)
		if (dst[i] != dst[j])
			dst[++j] = dst[i];
	count = j + 1;

	/*
	* remove self from list
	* send over any direct routes we have
	*
	* XXX we should attempt new routes here if RouteDirect is on.
	*/
	j = 0;
	for (i = 0; i < count; i++) {
		if (dst[i] == pvmmytid)
			continue;
		/* if ((pcbp = ttpcb_find(dst[i])) && pcbp->tt_state == TTOPEN) */
			/* mroute(pvmsbuf->m_mid, dst[i], tag, &ztv); */
		/* else */
			dst[j++] = dst[i];
	}
	count = j;      /* destinations still to go */

	/*
	* send rest of addresses to pvmd for distribution
	*/
	if (count > 0) {
		/* announce multicast address list to pvmd */
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		pvm_pkint(&count, 1, 1);
		pvm_pkint(dst, count, 1);
		sbf = pvm_setsbuf(sbf);
		if ((cc = mroute(sbf, TIDPVMD, TM_MCA, &ztv)) > 0)
			cc = 0;
		pvm_freebuf(sbf);

		/* send message */
		if (cc >= 0)
/*             if ((cc = mroute(pvmsbuf->m_mid, pvmmytid | TIDGID, tag, &ztv)) > 0) */
			if ((cc = mroute(mid, pvmmytid | TIDGID, tag, &ztv)) > 0)
				cc = 0;
	}

	PVM_FREE(dst);
	return cc;
}
