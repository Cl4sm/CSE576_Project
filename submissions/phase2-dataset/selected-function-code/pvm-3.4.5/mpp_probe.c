int
mpp_probe()
{
	struct pkt *pp, *pp2, *tosend;
	int dst;
	struct task *tp;
	int hasmsg;
	struct msgboxhdr *inbp = (struct msgboxhdr *)pvminbox;
	struct pidtidhdr *pvminfo = (struct pidtidhdr *)(infopage + PVMPAGEHDR);
	int ntids, i;

	tosend = ovfpkts;

	ovfpkts = pk_new(0);
	ovfpkts -> pk_link = ovfpkts -> pk_rlink = ovfpkts;

	while ((pp = tosend->pk_link) != tosend) {
		LISTDELETE(pp, pk_link, pk_rlink);
		dst = pp->pk_dst;
		if (tp = task_find(dst)) {
			if (tp->t_sock < 0) {
				if (mpp_output(tp, pp)) {
					for (pp = tosend->pk_link; pp != tosend; pp = pp2) {
						pp2 = pp->pk_link;
						if (pp->pk_dst == dst) {
							LISTDELETE(pp, pk_link, pk_rlink);
							LISTPUTBEFORE(ovfpkts, pp, pk_link, pk_rlink);
						}
					}
				}

			} else
				pkt_to_task(tp, pp);

		} else {
			sprintf(pvmtxt, "mpp_probe() pkt from t%x to t%x scrapped",
					pp->pk_src, pp->pk_dst);
			pvmlogperror(pvmtxt);
			pk_free(pp);
		}
	}

	pk_free(tosend);

	ntids = min(maxpidtid, ((struct shmpghdr *)infopage)->pg_ref);
	for (i = 0; i < ntids; i++)
		if (pidtids[i].pt_stat == ST_NOTREADY
		&& (tp = task_find(pidtids[i].pt_tid)) && tp->t_sock >= 0)
			pidtids[i].pt_stat = ST_SOCKET;

	hasmsg = (inbp->mb_read != inbp->mb_last);
	return hasmsg;
}
