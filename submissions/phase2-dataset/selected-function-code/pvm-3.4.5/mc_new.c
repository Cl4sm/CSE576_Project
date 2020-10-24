static struct pvmmclass *
mc_new(name)
	char *name;
{
	struct pvmmclass *np;

	if (np = TALLOC(1, struct pvmmclass, "pvmmclass")) {
		if (name) {
			np->mc_name = STRALLOC(name);
			np->mc_ent = me_new(-1);
			LISTPUTBEFORE(pvmmboxclasses, np, mc_link, mc_rlink);

		} else {
			np->mc_link = np->mc_rlink = np;
			np->mc_name = 0;
			np->mc_ent = 0;
		}
	}
	return np;
}
