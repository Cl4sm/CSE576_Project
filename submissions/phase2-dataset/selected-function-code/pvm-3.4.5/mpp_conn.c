int
mpp_conn(tp, tp2)
	struct task *tp;
	struct task *tp2;
{
	struct nodeset *sp;
	int pvminfo[SIZEHINFO];			/* host info */
	int ptypepart;					/* type field */

	if (pvmdebmask & PDMNODE) {
		sprintf(pvmtxt, "mpp_conn() pvmhost %x", tp2->t_tid);
		pvmlogerror(pvmtxt);
	}
	tp2->t_sock = tp->t_sock;
	tp2->t_sad = tp->t_sad;
	tp2->t_salen = tp->t_salen;
	tp2->t_flag |= TF_CONN;
	tp->t_sock = -1;
	for (sp = busynodes->n_link; sp != busynodes; sp = sp->n_link)
		if (sp->n_ptid == tp2->t_tid)
			break;
	if (sp == busynodes) {
		pvmlogerror("mpp_conn() task is not pvmhost\n");
		return -1;
	}
	ptypepart = (sp->n_ptype << (ffs(tidtmask) - 1)) | TIDONNODE;
	if (write(tp2->t_sock, sp2pvminfo, sizeof(sp2pvminfo)) 
	!= sizeof(sp2pvminfo)) {
		pvmlogperror("mpp_conn() write");
		return -1;
	}
	return 0;
}
