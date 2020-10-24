nmd_insert(name, req, data)
	char *name;
	int req;
	int data;
{
	struct nmdcls *np;
	struct nmdind *ep, *ep2;

	if (!(np = nc_find(name)))
		np = nc_new(name);

	if (req >= 0) {
		for (ep = np->nc_ent->ni_link; ep != np->nc_ent; ep = ep->ni_link)
			if (ep->ni_ind >= req)
				break;
		if (ep->ni_ind == req)
			ep = 0;

	} else {
		req = 0;
		for (ep = np->nc_ent->ni_link; ep != np->nc_ent; ep = ep->ni_link) {
			if (ep->ni_ind != req)
				break;
			req = ep->ni_ind + 1;
		}
	}

	if (ep) {
		ep2 = ni_new(req);
		ep2->ni_data = data;
		LISTPUTBEFORE(ep, ep2, ni_link, ni_rlink);

	} else
		req = -1;

	return req;
}
