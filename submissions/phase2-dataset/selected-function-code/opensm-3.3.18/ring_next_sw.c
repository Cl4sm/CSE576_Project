static
struct t_switch *ring_next_sw(struct t_switch *sw, unsigned cdir, int rdir)
{
	unsigned pt_grp, far_end = 0;

	if (!rdir)
		return NULL;
	/*
	 * Recall that links are installed into the torus so that their 1 end
	 * is in the "positive" coordinate direction relative to their 0 end
	 * (see link_tswitches() and connect_tlink()).  Recall also that for
	 * interswitch links, all links in a given switch port group have the
	 * same endpoints, so we just need to look at the first link.
	 */
	pt_grp = 2 * cdir;
	if (rdir > 0) {
		pt_grp++;
		far_end = 1;
	}

	if (!sw->ptgrp[pt_grp].port_cnt)
		return NULL;

	return sw->ptgrp[pt_grp].port[0]->link->end[far_end].sw;
}