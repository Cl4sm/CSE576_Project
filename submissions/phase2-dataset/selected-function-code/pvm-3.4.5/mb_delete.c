int
mb_delete(tid, name, req, flags)
	int tid;				/* owner task */
	char *name;				/* class name */
	int req;				/* index */
	int flags;				/* options */
{
	struct pvmmclass *np;
	struct pvmmentry *ep = 0;

	if (np = mc_find(name)) {
		for (ep = np->mc_ent->me_link; ep != np->mc_ent;
				ep = ep->me_link) {
			if (ep->me_ind == req)
				break;
		}
		if (ep == np->mc_ent)
			ep = 0;
	}

	if (!ep)
		return PvmNotFound;

	if ( !(ep->me_flags & PvmMboxOverWritable) && tid != ep->me_tid )
		return PvmDenied;

	req = ep->me_ind;
	me_free(np, ep);

	return req;
}
