int
mb_lookup(tid, name, req, flags, mpp)
	int tid;				/* owner task */
	char *name;				/* class name */
	int req;				/* index requested or -1 for first avail */
	int flags;
	struct pmsg **mpp;		/* message return */
{
	struct pvmmclass *np;
	struct pvmmentry *ep = 0;

	if (np = mc_find(name)) {
		for (ep = np->mc_ent->me_link; ep != np->mc_ent;
				ep = ep->me_link) {
			if (ep->me_ind >= req)
				break;
		}
		if (ep == np->mc_ent)
			ep = 0;
		else if (!(flags & PvmMboxFirstAvail) && ep->me_ind != req)
			ep = 0;
	}

	if (ep) {
		if ( flags & PvmMboxReadAndDelete ) {
			if ( (ep->me_flags & PvmMboxOverWritable)
					|| tid == ep->me_tid ) {
				req = ep->me_ind;
				ep->me_msg->m_ref++;
				*mpp = ep->me_msg;
				me_free(np, ep);
			} else {
				req = PvmDenied;
				*mpp = (struct pmsg *) NULL;
			}
		} else {
			req = ep->me_ind;
			ep->me_msg->m_ref++;
			*mpp = ep->me_msg;
		}
	} else
		req = PvmNotFound;

	return req;
}
