static
struct t_switch *alloc_tswitch(struct torus *t, struct f_switch *fsw)
{
	unsigned g;
	size_t new_sw_sz;
	struct t_switch *sw = NULL;
	void *ptr;

	if (!fsw)
		goto out;

	if (t->switch_cnt >= t->sw_pool_sz) {
		/*
		 * This should never happen, but occasionally a particularly
		 * pathological fabric can induce it.  So log an error.
		 */
		OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
			"ERR 4E14: unexpectedly requested too many switch "
			"structures!\n");
		goto out;
	}
	new_sw_sz = sizeof(*sw)
		+ fsw->port_cnt * sizeof(*sw->port)
		+ SWITCH_MAX_PORTGRPS * t->portgrp_sz * sizeof(*sw->ptgrp[0].port);
	sw = calloc(1, new_sw_sz);
	if (!sw) {
		OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
			"ERR 4E15: calloc: %s\n", strerror(errno));
		goto out;
	}
	sw->port = (void *)(sw + 1);
	sw->n_id = fsw->n_id;
	sw->port_cnt = fsw->port_cnt;
	sw->torus = t;
	sw->tmp = fsw;

	ptr = &sw->port[sw->port_cnt];

	for (g = 0; g < SWITCH_MAX_PORTGRPS; g++) {
		sw->ptgrp[g].port_grp = g;
		sw->ptgrp[g].sw = sw;
		sw->ptgrp[g].port = ptr;
		ptr = &sw->ptgrp[g].port[t->portgrp_sz];
	}
	t->sw_pool[t->switch_cnt++] = sw;
out:
	return sw;
}