int
nmd_delete(name, req)
	char *name;
	int req;
{
	struct nmdcls *np;
	struct nmdind *ep = 0;

	if (np = nc_find(name)) {
		if (req >= 0) {
			for (ep = np->nc_ent->ni_link; ep != np->nc_ent; ep = ep->ni_link)
				if (ep->ni_ind == req)
					break;
			if (ep == np->nc_ent)
				ep = 0;
		}
	}

	if (ep) {
		req = ep->ni_ind;
		LISTDELETE(ep, ni_link, ni_rlink);
		PVM_FREE(ep);
		if (np->nc_ent->ni_link == np->nc_ent) {
			LISTDELETE(np, nc_link, nc_rlink);
			PVM_FREE(np->nc_name);
			PVM_FREE(np->nc_ent);
			PVM_FREE(np);
		}
	} else
		req = -1;

	return req;
}
