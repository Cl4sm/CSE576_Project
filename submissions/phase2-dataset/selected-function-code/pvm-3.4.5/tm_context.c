tm_context(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	struct pmsg *mp2;
	struct ccon *cp;
	int cid;
	int cc;

	mp2 = replymessage(mp);
	if (upkuint(mp, &cid)) {
		cc = PvmBadParam;

	} else {
		if (cid == 0) {
			if (cp = ccon_new(tp))
				cc = cp->c_cid;
			else
				cc = PvmOutOfRes;

		} else {
			cc = PvmNotFound;
			if (tp->t_ccs) {
				FORLIST (cp, tp->t_ccs, c_peer) {
					if (cp->c_cid == cid) {
						LISTDELETE(cp, c_link, c_rlink);
						LISTDELETE(cp, c_peer, c_rpeer);
						PVM_FREE(cp);
						cc = 0;
						break;
					}
				}
			}
		}
	}
	pkint(mp2, cc);
	sendmessage(mp2);
	return 0;
}
