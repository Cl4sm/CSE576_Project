static
bool connect_tlink(struct port_grp *pg0, struct endpoint *f_ep0,
		   struct port_grp *pg1, struct endpoint *f_ep1,
		   struct torus *t)
{
	struct link *l;
	bool success = false;

	if (pg0->port_cnt == t->portgrp_sz) {
		OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
			"ERR 4E17: exceeded port group max "
			"port count (%d): switch GUID 0x%04"PRIx64"\n",
			t->portgrp_sz, cl_ntoh64(pg0->sw->n_id));
		goto out;
	}
	if (pg1->port_cnt == t->portgrp_sz) {
		OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
			"ERR 4E18: exceeded port group max "
			"port count (%d): switch GUID 0x%04"PRIx64"\n",
			t->portgrp_sz, cl_ntoh64(pg1->sw->n_id));
		goto out;
	}
	l = alloc_tlink(t);
	if (!l)
		goto out;

	l->end[0].type = f_ep0->type;
	l->end[0].port = f_ep0->port;
	l->end[0].n_id = f_ep0->n_id;
	l->end[0].sw = pg0->sw;
	l->end[0].link = l;
	l->end[0].pgrp = pg0;
	pg0->port[pg0->port_cnt++] = &l->end[0];
	pg0->sw->port[f_ep0->port] = &l->end[0];

	if (f_ep0->osm_port) {
		l->end[0].osm_port = f_ep0->osm_port;
		l->end[0].osm_port->priv = &l->end[0];
		f_ep0->osm_port = NULL;
	}

	l->end[1].type = f_ep1->type;
	l->end[1].port = f_ep1->port;
	l->end[1].n_id = f_ep1->n_id;
	l->end[1].sw = pg1->sw;
	l->end[1].link = l;
	l->end[1].pgrp = pg1;
	pg1->port[pg1->port_cnt++] = &l->end[1];
	pg1->sw->port[f_ep1->port] = &l->end[1];

	if (f_ep1->osm_port) {
		l->end[1].osm_port = f_ep1->osm_port;
		l->end[1].osm_port->priv = &l->end[1];
		f_ep1->osm_port = NULL;
	}
	/*
	 * Disconnect fabric link, so that later we can see if any were
	 * left unconnected in the torus.
	 */
	((struct f_switch *)f_ep0->sw)->port[f_ep0->port] = NULL;
	f_ep0->sw = NULL;
	f_ep0->port = -1;

	((struct f_switch *)f_ep1->sw)->port[f_ep1->port] = NULL;
	f_ep1->sw = NULL;
	f_ep1->port = -1;

	success = true;
out:
	return success;
}