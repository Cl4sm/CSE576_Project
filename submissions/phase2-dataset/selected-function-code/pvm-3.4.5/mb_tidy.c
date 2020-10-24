mb_tidy(tid)
	int tid;
{
	struct pvmmclass *np, *np2;
	struct pvmmentry *ep, *ep2;

	if (!tid)
		return 0;

	if (pvmdebmask & PDMMBOX)
		pvmlogprintf("mb_tidy() tid 0x%x\n", tid);

	for (np = pvmmboxclasses->mc_link; np != pvmmboxclasses; np = np2) {
		np2 = np->mc_link;
		for (ep = np->mc_ent->me_link; ep != np->mc_ent; ep = ep2) {
			ep2 = ep->me_link;
			if (ep->me_tid == tid) {
				if (ep->me_flags & PvmMboxPersistent) {
					ep->me_savetid = ep->me_tid;
					ep->me_tid = 0;
				} else {
					me_free(np, ep);
					if (np2->mc_rlink != np)
						break;
				}
			}
		}
	}
	return 0;
}
