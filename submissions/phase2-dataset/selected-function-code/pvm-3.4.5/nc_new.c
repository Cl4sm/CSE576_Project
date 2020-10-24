nc_new(name)
	char *name;
{
	struct nmdcls *np;

	if (np = TALLOC(1, struct nmdcls, "nmdc")) {
		if (name) {
			np->nc_name = STRALLOC(name);
			np->nc_ent = ni_new(-1);
			LISTPUTBEFORE(classes, np, nc_link, nc_rlink);

		} else {
			np->nc_link = np->nc_rlink = np;
			np->nc_name = 0;
			np->nc_ent = 0;
		}
	}
	return np;
}
