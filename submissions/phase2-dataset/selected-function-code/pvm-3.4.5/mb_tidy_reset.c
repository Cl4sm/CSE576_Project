mb_tidy_reset(tid)
	int tid;
{
	struct pvmmclass *np, *np2;
	struct pvmmentry *ep, *ep2;

	if (!tid)
		return 0;

	if (pvmdebmask & PDMMBOX)
		pvmlogprintf("mb_tidy_reset() tid 0x%x\n", tid);

	for (np = pvmmboxclasses->mc_link; np != pvmmboxclasses; np = np2) {
		np2 = np->mc_link;
		for (ep = np->mc_ent->me_link; ep != np->mc_ent; ep = ep2) {
			ep2 = ep->me_link;
			/* mb_tidy() & mb_tidy_reset() could be in any order... */
			if ( (ep->me_flags & PvmMboxPersistent)
					&& ( ep->me_tid == tid
						|| (!(ep->me_tid) && ep->me_savetid == tid) ) )
			{
				me_free(np, ep);
				if (np2->mc_rlink != np)
					break;
			}
		}
	}
	return 0;
}
