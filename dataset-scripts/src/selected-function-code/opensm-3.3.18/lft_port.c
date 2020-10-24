static
int lft_port(struct torus *t,
	     struct t_switch *sw, struct t_switch *dsw,
	     bool update_port_cnt, bool ca)
{
	unsigned g, p;
	struct port_grp *pg;

	/*
	 * The IBA does not provide a way to preserve path history for
	 * routing decisions and VL assignment, and the only mechanism to
	 * provide global fabric knowledge to the routing engine is via
	 * the four SL bits.  This severely constrains the ability to deal
	 * with missing/dead switches.
	 *
	 * Also, if routing a torus with XYZ-DOR, the only way to route
	 * around a missing/dead switch is to introduce a turn that is
	 * illegal under XYZ-DOR.
	 *
	 * But here's what we can do:
	 *
	 * We have a VL bit we use to flag illegal turns, thus putting the
	 * hop directly after an illegal turn on a separate set of VLs.
	 * Unfortunately, since there is no path history,  the _second_
	 * and subsequent hops after an illegal turn use the standard
	 * XYZ-DOR VL set.  This is enough to introduce credit loops in
	 * many cases.
	 *
	 * To minimize the number of cases such illegal turns can introduce
	 * credit loops, we try to introduce the illegal turn as late in a
	 * path as possible.
	 *
	 * Define a turning switch as a switch where a path turns from one
	 * coordinate direction onto another.  If a turning switch in a path
	 * is missing, construct the LFT entries so that the path progresses
	 * as far as possible on the shortest path to the turning switch.
	 * When progress is not possible, turn onto the next coordinate
	 * direction.
	 *
	 * The next turn after that will be an illegal turn, after which
	 * point the path will continue to use a standard XYZ-DOR path.
	 */
	if (dsw->i != sw->i) {

		if (next_hop_x(t, sw, dsw, &g))
			goto done;
		/*
		 * This path has made as much progress in this direction as
		 * is possible, so turn it now.
		 */
		if (dsw->j != sw->j && next_hop_y(t, sw, dsw, &g))
			goto done;

		if (dsw->k != sw->k && next_hop_z(t, sw, dsw, &g))
			goto done;

		warn_on_routing("Error: unable to route", sw, dsw);
		goto no_route;
	} else if (dsw->j != sw->j) {

		if (next_hop_y(t, sw, dsw, &g))
			goto done;

		if (dsw->k != sw->k && next_hop_z(t, sw, dsw, &g))
			goto done;

		warn_on_routing("Error: unable to route", sw, dsw);
		goto no_route;
	} else {
		if (dsw->k == sw->k)
			warn_on_routing("Warning: bad routing", sw, dsw);

		if (next_hop_z(t, sw, dsw, &g))
			goto done;

		warn_on_routing("Error: unable to route", sw, dsw);
		goto no_route;
	}
done:
	pg = &sw->ptgrp[g];
	if (!pg->port_cnt)
		goto no_route;

	if (update_port_cnt) {
		if (ca)
			p = pg->ca_dlid_cnt++ % pg->port_cnt;
		else
			p = pg->sw_dlid_cnt++ % pg->port_cnt;
	} else {
		/*
		 * If we're not updating port counts, then we're just running
		 * routes for SL path checking, and it doesn't matter which
		 * of several parallel links we use.  Use the first one.
		 */
		p = 0;
	}
	p = pg->port[p]->port;

	return p;

no_route:
	/*
	 * We can't get there from here.
	 */
	OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
		"ERR 4E39: routing on sw 0x%04"PRIx64": sending "
		"traffic for dest sw 0x%04"PRIx64" to port %u\n",
		cl_ntoh64(sw->n_id), cl_ntoh64(dsw->n_id), OSM_NO_PATH);
	return -1;
}