static
struct link *find_f_link(struct fabric *f,
			 guid_t guid0, int port0, guid_t guid1, int port1)
{
	unsigned l;
	struct link *link;

	if (f->link) {
		for (l = 0; l < f->link_cnt; l++) {
			link = f->link[l];
			if ((link->end[0].n_id == guid0 &&
			     link->end[0].port == port0 &&
			     link->end[1].n_id == guid1 &&
			     link->end[1].port == port1) ||
			    (link->end[0].n_id == guid1 &&
			     link->end[0].port == port1 &&
			     link->end[1].n_id == guid0 &&
			     link->end[1].port == port0))
				return link;
		}
	}
	return NULL;
}