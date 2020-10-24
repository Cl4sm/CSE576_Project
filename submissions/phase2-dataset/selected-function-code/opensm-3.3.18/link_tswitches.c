static
bool link_tswitches(struct torus *t, int cdir,
		    struct t_switch *t_sw0, struct t_switch *t_sw1)
{
	int p;
	struct port_grp *pg0, *pg1;
	struct f_switch *f_sw0, *f_sw1;
	const char *cdir_name = "unknown";
	unsigned port_cnt;
	int success = false;

	/*
	 * If this is a 2D torus, it is possible for this function to be
	 * called with its two switch arguments being the same switch, in
	 * which case there are no links to install.
	 */
	if (t_sw0 == t_sw1 &&
	    ((cdir == 0 && t->x_sz == 1) ||
	     (cdir == 1 && t->y_sz == 1) ||
	     (cdir == 2 && t->z_sz == 1))) {
		success = true;
		goto out;
	}
	/*
	 * Ensure that t_sw1 is in the positive cdir direction wrt. t_sw0.
	 * ring_next_sw() relies on it.
	 */
	switch (cdir) {
	case 0:
		if (t->x_sz > 1 &&
		    canonicalize(t_sw0->i + 1, t->x_sz) != t_sw1->i) {
			cdir_name = "x";
			goto cdir_error;
		}
		break;
	case 1:
		if (t->y_sz > 1 &&
		    canonicalize(t_sw0->j + 1, t->y_sz) != t_sw1->j) {
			cdir_name = "y";
			goto cdir_error;
		}
		break;
	case 2:
		if (t->z_sz > 1 &&
		    canonicalize(t_sw0->k + 1, t->z_sz) != t_sw1->k) {
			cdir_name = "z";
			goto cdir_error;
		}
		break;
	default:
	cdir_error:
		OSM_LOG(&t->osm->log, OSM_LOG_ERROR, "ERR 4E19: "
			"sw 0x%04"PRIx64" (%d,%d,%d) <--> "
			"sw 0x%04"PRIx64" (%d,%d,%d) "
			"invalid torus %s link orientation\n",
			cl_ntoh64(t_sw0->n_id), t_sw0->i, t_sw0->j, t_sw0->k,
			cl_ntoh64(t_sw1->n_id), t_sw1->i, t_sw1->j, t_sw1->k,
			cdir_name);
		goto out;
	}

	f_sw0 = t_sw0->tmp;
	f_sw1 = t_sw1->tmp;

	if (!f_sw0 || !f_sw1) {
		OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
			"ERR 4E1A: missing fabric switches!\n"
			"  switch GUIDs: 0x%04"PRIx64" 0x%04"PRIx64"\n",
			cl_ntoh64(t_sw0->n_id), cl_ntoh64(t_sw1->n_id));
		goto out;
	}
	pg0 = &t_sw0->ptgrp[2*cdir + 1];
	pg0->type = PASSTHRU;

	pg1 = &t_sw1->ptgrp[2*cdir];
	pg1->type = PASSTHRU;

	port_cnt = f_sw0->port_cnt;
	/*
	 * Find all the links between these two switches.
	 */
	for (p = 0; p < port_cnt; p++) {
		struct endpoint *f_ep0 = NULL, *f_ep1 = NULL;

		if (!f_sw0->port[p] || !f_sw0->port[p]->link)
			continue;

		if (f_sw0->port[p]->link->end[0].n_id == t_sw0->n_id &&
		    f_sw0->port[p]->link->end[1].n_id == t_sw1->n_id) {

			f_ep0 = &f_sw0->port[p]->link->end[0];
			f_ep1 = &f_sw0->port[p]->link->end[1];
		} else if (f_sw0->port[p]->link->end[1].n_id == t_sw0->n_id &&
			   f_sw0->port[p]->link->end[0].n_id == t_sw1->n_id) {

			f_ep0 = &f_sw0->port[p]->link->end[1];
			f_ep1 = &f_sw0->port[p]->link->end[0];
		} else
			continue;

		if (!(f_ep0->type == PASSTHRU && f_ep1->type == PASSTHRU)) {
			OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
				"ERR 4E1B: not interswitch "
				"link:\n  0x%04"PRIx64"/%d <-> 0x%04"PRIx64"/%d\n",
				cl_ntoh64(f_ep0->n_id), f_ep0->port,
				cl_ntoh64(f_ep1->n_id), f_ep1->port);
			goto out;
		}
		/*
		 * Skip over links that already have been established in the
		 * torus.
		 */
		if (!(f_ep0->sw && f_ep1->sw))
			continue;

		if (!connect_tlink(pg0, f_ep0, pg1, f_ep1, t))
			goto out;
	}
	success = true;
out:
	return success;
}