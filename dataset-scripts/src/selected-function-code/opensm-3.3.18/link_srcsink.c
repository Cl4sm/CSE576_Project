static
bool link_srcsink(struct torus *t, int i, int j, int k)
{
	struct endpoint *f_ep0;
	struct endpoint *f_ep1;
	struct t_switch *tsw;
	struct f_switch *fsw;
	struct port_grp *pg;
	struct link *fl, *tl;
	unsigned p, port_cnt;
	bool success = false;

	i = canonicalize(i, t->x_sz);
	j = canonicalize(j, t->y_sz);
	k = canonicalize(k, t->z_sz);

	tsw = t->sw[i][j][k];
	if (!tsw)
		return true;

	fsw = tsw->tmp;
	/*
	 * link_srcsink is supposed to get called once for every switch in
	 * the fabric.  At this point every fsw we encounter must have a
	 * non-null osm_switch.  Otherwise something has gone horribly
	 * wrong with topology discovery; the most likely reason is that
	 * the fabric contains a radix-4 torus dimension, but the user gave
	 * a config that didn't say so, breaking all the checking in
	 * safe_x_perpendicular and friends.
	 */
	if (!(fsw && fsw->osm_switch)) {
		OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
			"ERR 4E1C: Invalid topology discovery. "
			"Verify torus-2QoS.conf contents.\n");
		return false;
	}

	pg = &tsw->ptgrp[2 * TORUS_MAX_DIM];
	pg->type = SRCSINK;
	tsw->osm_switch = fsw->osm_switch;
	tsw->osm_switch->priv = tsw;
	fsw->osm_switch = NULL;

	port_cnt = fsw->port_cnt;
	for (p = 0; p < port_cnt; p++) {

		if (!fsw->port[p])
			continue;

		if (fsw->port[p]->type == SRCSINK) {
			/*
			 * If the endpoint is the switch port used for in-band
			 * communication with the switch itself, move it to
			 * the torus.
			 */
			if (pg->port_cnt == t->portgrp_sz) {
				OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
					"ERR 4E1D: exceeded port group max port "
					"count (%d): switch GUID 0x%04"PRIx64"\n",
					t->portgrp_sz, cl_ntoh64(tsw->n_id));
				goto out;
			}
			fsw->port[p]->sw = tsw;
			fsw->port[p]->pgrp = pg;
			tsw->port[p] = fsw->port[p];
			tsw->port[p]->osm_port->priv = tsw->port[p];
			pg->port[pg->port_cnt++] = fsw->port[p];
			fsw->port[p] = NULL;

		} else if (fsw->port[p]->link &&
			   fsw->port[p]->type == PASSTHRU) {
			/*
			 * If the endpoint is a link to a CA, create a new link
			 * in the torus.  Disconnect the fabric link.
			 */

			fl = fsw->port[p]->link;

			if (fl->end[0].sw == fsw) {
				f_ep0 = &fl->end[0];
				f_ep1 = &fl->end[1];
			} else if (fl->end[1].sw == fsw) {
				f_ep1 = &fl->end[0];
				f_ep0 = &fl->end[1];
			} else
				continue;

			if (f_ep1->type != SRCSINK)
				continue;

			if (pg->port_cnt == t->portgrp_sz) {
				OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
					"ERR 4E1E: exceeded port group max port "
					"count (%d): switch GUID 0x%04"PRIx64"\n",
					t->portgrp_sz, cl_ntoh64(tsw->n_id));
				goto out;
			}
			/*
			 * Switch ports connected to links don't get
			 * associated with osm_port_t objects; see
			 * capture_fabric().  So just check CA end.
			 */
			if (!f_ep1->osm_port) {
				OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
					"ERR 4E1F: NULL osm_port->priv port "
					"GUID 0x%04"PRIx64"\n",
					cl_ntoh64(f_ep1->n_id));
				goto out;
			}
			tl = alloc_tlink(t);
			if (!tl)
				continue;

			tl->end[0].type = f_ep0->type;
			tl->end[0].port = f_ep0->port;
			tl->end[0].n_id = f_ep0->n_id;
			tl->end[0].sw = tsw;
			tl->end[0].link = tl;
			tl->end[0].pgrp = pg;
			pg->port[pg->port_cnt++] = &tl->end[0];
			pg->sw->port[f_ep0->port] =  &tl->end[0];

			tl->end[1].type = f_ep1->type;
			tl->end[1].port = f_ep1->port;
			tl->end[1].n_id = f_ep1->n_id;
			tl->end[1].sw = NULL;	/* Correct for a CA */
			tl->end[1].link = tl;
			tl->end[1].pgrp = NULL;	/* Correct for a CA */

			tl->end[1].osm_port = f_ep1->osm_port;
			tl->end[1].osm_port->priv = &tl->end[1];
			f_ep1->osm_port = NULL;

			t->ca_cnt++;
			f_ep0->sw = NULL;
			f_ep0->port = -1;
			fsw->port[p] = NULL;
		}
	}
	success = true;
out:
	return success;
}