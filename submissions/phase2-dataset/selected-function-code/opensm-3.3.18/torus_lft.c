static
bool torus_lft(struct torus *t, struct t_switch *sw)
{
	bool success = true;
	int dp;
	unsigned p, s;
	uint16_t l, dlid_base;
	uint8_t dlid_lmc;
	bool ca;
	struct port_grp *pgrp;
	struct t_switch *dsw;
	osm_switch_t *osm_sw;
	uint8_t order[IB_NODE_NUM_PORTS_MAX+1];

	if (!(sw->osm_switch && sw->osm_switch->priv == sw)) {
		OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
			"ERR 4E3D: sw->osm_switch->priv != sw "
			"for sw 0x%04"PRIx64"\n", cl_ntoh64(sw->n_id));
		return false;
	}
	osm_sw = sw->osm_switch;
	memset(osm_sw->new_lft, OSM_NO_PATH, osm_sw->lft_size);

	for (s = 0; s < t->switch_cnt; s++) {

		dsw = t->sw_pool[s];
		pgrp = &dsw->ptgrp[2 * TORUS_MAX_DIM];

		memset(order, IB_INVALID_PORT_NUM, sizeof(order));
		for (p = 0; p < pgrp->port_cnt; p++)
			order[pgrp->port[p]->port] = p;

		for (p = 0; p < ARRAY_SIZE(order); p++) {

			uint8_t px = order[t->port_order[p]];

			if (px == IB_INVALID_PORT_NUM)
				continue;

			if (!get_lid(pgrp, px, &dlid_base, &dlid_lmc, &ca))
				return false;

			if (sw->n_id == dsw->n_id)
				dp = pgrp->port[px]->port;
			else
				dp = lft_port(t, sw, dsw, true, ca);
			/*
			 * LMC > 0 doesn't really make sense for torus-2QoS.
			 * So, just make sure traffic gets delivered if
			 * non-zero LMC is used.
			 */
			if (dp >= 0)
				for (l = 0; l < (1U << dlid_lmc); l++)
					osm_sw->new_lft[dlid_base + l] = dp;
			else
				success = false;
		}
	}
	return success;
}