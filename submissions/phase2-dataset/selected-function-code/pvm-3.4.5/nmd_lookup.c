nmd_lookup(name, req, datap)
	char *name;
	int req;
	int *datap;
{
	struct nmdcls *np;
	struct nmdind *ep = 0;

	if (np = nc_find(name)) {
		if (req >= 0) {
			for (ep = np->nc_ent->ni_link; ep != np->nc_ent; ep = ep->ni_link)
				if (ep->ni_ind == req)
					break;
		} else
			ep = np->nc_ent->ni_link;

		if (ep == np->nc_ent)
			ep = 0;
	}

	if (ep) {
		req = ep->ni_ind;
		*datap = ep->ni_data;

	} else
		req = -1;

	return req;
}
