static
bool get_lid(struct port_grp *pg, unsigned p,
	     uint16_t *dlid_base, uint8_t *dlid_lmc, bool *ca)
{
	struct endpoint *ep;
	osm_port_t *osm_port;

	if (p >= pg->port_cnt) {
		OSM_LOG(&pg->sw->torus->osm->log, OSM_LOG_ERROR,
			"ERR 4E3A: Port group index %u too large: sw "
			"0x%04"PRIx64" pt_grp %u pt_grp_cnt %u\n",
			p, cl_ntoh64(pg->sw->n_id),
			(unsigned)pg->port_grp, (unsigned)pg->port_cnt);
		return false;
	}
	if (pg->port[p]->type == SRCSINK) {
		ep = pg->port[p];
		if (ca)
			*ca = false;
	} else if (pg->port[p]->type == PASSTHRU &&
		   pg->port[p]->link->end[1].type == SRCSINK) {
		/*
		 * If this port is connected via a link to a CA, then we
		 * know link->end[0] is the switch end and link->end[1] is
		 * the CA end; see build_ca_link() and link_srcsink().
		 */
		ep = &pg->port[p]->link->end[1];
		if (ca)
			*ca = true;
	} else {
		OSM_LOG(&pg->sw->torus->osm->log, OSM_LOG_ERROR,
			"ERR 4E3B: Switch 0x%04"PRIx64" port %d improperly connected\n",
			cl_ntoh64(pg->sw->n_id), pg->port[p]->port);
		return false;
	}
	osm_port = ep->osm_port;
	if (!(osm_port && osm_port->priv == ep)) {
		OSM_LOG(&pg->sw->torus->osm->log, OSM_LOG_ERROR,
			"ERR 4E3C: ep->osm_port->priv != ep "
			"for sw 0x%04"PRIx64" port %d\n",
			cl_ntoh64(((struct t_switch *)(ep->sw))->n_id), ep->port);
		return false;
	}
	*dlid_base = cl_ntoh16(osm_physp_get_base_lid(osm_port->p_physp));
	*dlid_lmc = osm_physp_get_lmc(osm_port->p_physp);

	return true;
}