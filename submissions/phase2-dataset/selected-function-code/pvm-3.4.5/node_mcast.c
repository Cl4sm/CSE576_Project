node_mcast(mid, dtid, code)
	int mid;	/* message id */
	int dtid;	/* destination */
	int code;	/* type */
{
	static int *tids = 0;	/* intended recipients of multicast message */
	static int ntids = -1;
	static int ntask;		/* number of tids */
	static struct timeval ztv = { 0, 0 };

	int i;
	long count = 0;
	int cc = 0;
	int dummy;

	/* intercept multicast info */

	if (dtid == TIDPVMD) {
		int sbf = mid;

		pvm_setrbuf(mid);
		pvm_upkint(&ntask, 1, 1);
		if (ntask > ntids) {
			if (tids)
				PVM_FREE(tids);
			tids = TALLOC(ntask, int, "tids");
			ntids = ntask;
		}
		pvm_upkint(tids, ntask, 1);
		/* sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo)); */
		pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		dummy = TIDGID;
		pvm_pkint(&dummy, 1, 1);
		pvm_setrbuf(pvm_setsbuf(sbf));
		return 0;
	}

	for (i = 0; i < ntask; i++)
		if (tids[i] != pvmmytid)
			cc = mroute(mid, tids[i], code, &ztv);

	ntask = 0;

	return cc;
}
