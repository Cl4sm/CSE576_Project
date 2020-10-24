ccon_new(tp)
	struct task *tp;
{
	static int lastcid = 0;

	int startcid;
	int cid;
	struct ccon *cp, *cp2;

	if (++lastcid > tidlmask - NUM_SYSCTX)
		lastcid = 1;
	startcid = lastcid;
	cp = cconlist;

	while (1) {
		cid = myhostpart + lastcid;
		while (cp->c_cid < cid)
			if ((cp = cp->c_link) == cconlist)
				break;

		if (cp->c_cid != cid)
			break;

		if (++lastcid > tidlmask) {
			lastcid = 1;
			cp = cconlist;
		}
		if (lastcid == startcid) {
			pvmlogprintf("ccon_new() out of cids?\n");
			return 0;
		}
	}

	if (!(cp2 = TALLOC(1, struct ccon, "ccon"))) {
		pvmlogprintf("ccon_new() can't get memory\n");
		return 0;
	}
	cp2->c_cid = cid;
	cp2->c_tid = tp->t_tid;
	LISTPUTBEFORE(tp->t_ccs, cp2, c_peer, c_rpeer);
	LISTPUTBEFORE(cp, cp2, c_link, c_rlink);
	if (pvmdebmask & PDMTASK) {
		pvmlogprintf("ccon_new() tid=%x cid=%x\n", cp2->c_tid, cp2->c_cid);
	}
	return cp2;
}
