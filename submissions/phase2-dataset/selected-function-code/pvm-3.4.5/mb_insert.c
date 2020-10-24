int
mb_insert(tid, name, req, flags, mp)
	int tid;				/* owner task */
	char *name;				/* class name */
	int req;				/* index requested or -1 for lowest avail */
	int flags;
	struct pmsg *mp;		/* message to store */
{
	struct pvmmclass *np;
	struct pvmmentry *ep;
	struct pvmmentry *ep2 = 0;

	int dind;	/* direct index	*/
	int cind;	/* current index */

	/* must change to always pass in -1 for 3.4 calls */
	/* then can distinguish between 3.4 reinsert which succeeds
		if !overwritable/minst & 3.3 reinsert which should fail */
	/* to fix the "if (ep->me_ind == req)" conditional
		use a "current index":  cind = ( req == -1 ) ? 0 : req; */
	/* also, minst supercedes overwritable, else no way to build
		a minst list from within a single task... */

	if (!(np = mc_find(name)))
		if (!(np = mc_new(name)))
			return PvmNoMem;

	dind = PvmMboxDirectIndexOf( flags );

	cind = ( req == -1 ) ?
		( ( dind ) ? dind : 0 )
		: req;

	for (ep = np->mc_ent->me_link; ep != np->mc_ent; ep = ep->me_link)
		if (ep->me_ind >= cind)
			break;

	/* default insert is "locked"... */

	if (ep->me_ind == cind) {
		if (flags & PvmMboxMultiInstance) {
			if ( req != -1 ) /* hack for 3.3 compat */
				return PvmDenied;
			for (; ep != np->mc_ent; ep = ep->me_link) {
				if (ep->me_ind != cind)
					break;
				cind++;
			}
		} else {
			if ( !(ep->me_flags & PvmMboxOverWritable)
					&& tid != ep->me_tid) {
				return PvmDenied;
			}
			ep2 = ep;
		}
	}

	if (ep2) {
		ep = ep2->me_link;
		LISTDELETE(ep2, me_link, me_rlink);
		pmsg_unref(ep2->me_msg);
		PVM_FREE(ep2);
	}

	ep2 = me_new(cind);
	ep2->me_tid = tid;
	ep2->me_flags = flags;
	ep2->me_msg = mp;
	LISTPUTBEFORE(ep, ep2, me_link, me_rlink);

	return cind;
}
