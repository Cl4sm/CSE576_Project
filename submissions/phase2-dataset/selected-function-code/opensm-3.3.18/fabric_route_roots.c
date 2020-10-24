static void fabric_route_roots(IN ftree_fabric_t * p_ftree)
{
	uint16_t lid;
	uint8_t port_num;
	osm_port_t *p_port;
	ftree_sw_t *p_sw;
	ftree_sw_t *p_leaf_sw;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	/*
	 * We need a switch that will accomodate all the down/up turns in
	 * the fabric. Having these turn in a single place in the fabric
	 * will not create credit loops.
	 * So we need to select this switch.
	 * The idea here is to chose leaf with the highest index. I don't
	 * have any theory to back me up on this. It's just a general thought
	 * that this way the switch that might be a bottleneck for many mcast
	 * groups will be far away from the OpenSM, so it will draw the
	 * multicast traffic away from the SM.
	 */

	p_leaf_sw = p_ftree->leaf_switches[p_ftree->leaf_switches_num-1];

	/*
	 * Now go over all the switches in the fabric that
	 * have lower rank, and route the missing LIDs to
	 * the selected leaf switch.
	 * In short, this leaf switch now poses a target
	 * for all those missing LIDs.
	 */

	for (p_sw = (ftree_sw_t *) cl_qmap_head(&p_ftree->sw_tbl);
	     p_sw != (ftree_sw_t *) cl_qmap_end(&p_ftree->sw_tbl);
	     p_sw = (ftree_sw_t *) cl_qmap_next(&p_sw->map_item)) {

		if (p_sw->rank >= p_ftree->leaf_switch_rank)
			continue;

		for (lid = 1; lid <= p_leaf_sw->p_osm_sw->max_lid_ho; lid ++) {

			if (p_sw->p_osm_sw->new_lft[lid] != OSM_NO_PATH ||
			    p_leaf_sw->hops[lid] == OSM_NO_PATH)
				continue;

			p_port = osm_get_port_by_lid_ho(&p_ftree->p_osm->subn,
							lid);

			/* we're interested only in switches */
			if (!p_port || !p_port->p_node->sw)
				continue;

			/*
			 * the missing LID will be routed through the same
			 * port that routes to the selected leaf switch
			 */
			port_num = p_sw->p_osm_sw->new_lft[p_leaf_sw->base_lid];

			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
				"Switch %s: setting path to LID %u "
				"through port %u\n",
				tuple_to_str(p_sw->tuple), lid, port_num);

			/* set local lft */
			p_sw->p_osm_sw->new_lft[lid] = port_num;

			/*
			 * Set local min hop table.
			 * The distance to the target LID is a distance
			 * to the selected leaf switch plus the distance
			 * from the leaf to the target LID.
			 */
			sw_set_hops(p_sw, lid, port_num,
				p_sw->hops[p_leaf_sw->base_lid] +
				p_leaf_sw->hops[lid], TRUE);
		}
	}

	OSM_LOG_EXIT(&p_ftree->p_osm->log);
}				/* fabric_route_roots() */