static float mcast_mgr_compute_max_hops(osm_sm_t * sm, cl_qmap_t * m,
					const osm_switch_t * this_sw)
{
	uint32_t max_hops = 0, hops;
	uint16_t lid;
	cl_map_item_t *i;
	osm_switch_t *sw;

	OSM_LOG_ENTER(sm->p_log);

	/*
	   For each member of the multicast group, compute the
	   number of hops to its base LID.
	 */
	for (i = cl_qmap_head(m); i != cl_qmap_end(m); i = cl_qmap_next(i)) {
		sw = cl_item_obj(i, sw, mgrp_item);
		lid = cl_ntoh16(osm_node_get_base_lid(sw->p_node, 0));
		hops = osm_switch_get_least_hops(this_sw, lid);
		if (!sw->is_mc_member)
			hops += 1;
		if (hops > max_hops)
			max_hops = hops;
	}

	/* Note that at this point we might get (max_hops == 0),
	   which means that there's only one member in the mcast
	   group, and it's the current switch */

	OSM_LOG_EXIT(sm->p_log);
	return (float)max_hops;
}