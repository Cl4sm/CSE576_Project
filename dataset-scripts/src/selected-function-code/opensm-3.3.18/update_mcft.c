static int update_mcft(osm_sm_t * p_sm, vertex_t * adj_list,
		       uint32_t adj_list_size, uint16_t mlid_ho,
		       cl_qmap_t * port_map, osm_switch_t * root_sw)
{
	uint32_t i = 0;
	uint8_t port = 0, remote_port = 0;
	uint8_t upstream_port = 0, downstream_port = 0;
	ib_net64_t guid = 0;
	osm_switch_t *p_sw = NULL;
	osm_node_t *remote_node = NULL;
	osm_physp_t *p_physp = NULL;
	osm_mcast_tbl_t *p_tbl = NULL;
	vertex_t *curr_adj = NULL;

	OSM_LOG_ENTER(p_sm->p_log);

	for (i = 1; i < adj_list_size; i++) {
		p_sw = adj_list[i].sw;
		OSM_LOG(p_sm->p_log, OSM_LOG_VERBOSE,
			"Processing switch 0x%016" PRIx64
			" (%s) for MLID 0x%X\n", cl_ntoh64(adj_list[i].guid),
			p_sw->p_node->print_desc, mlid_ho);

		/* if a) the switch does not support mcast  or
		      b) no ports of this switch are part or the mcast group
		   then cycle
		 */
		if (osm_switch_supports_mcast(p_sw) == FALSE ||
		    (p_sw->num_of_mcm == 0 && !(p_sw->is_mc_member)))
			continue;

		p_tbl = osm_switch_get_mcast_tbl_ptr(p_sw);

		/* add all ports of this sw to the mcast table,
		   if they are part of the mcast grp
		 */
		if (p_sw->is_mc_member)
			osm_mcast_tbl_set(p_tbl, mlid_ho, 0);
		for (port = 1; port < p_sw->num_ports; port++) {
			/* get the node behind the port */
			remote_node =
				osm_node_get_remote_node(p_sw->p_node, port,
							 &remote_port);
			/* check if connected and its not the same switch */
			if (!remote_node || remote_node->sw == p_sw)
				continue;
			/* make sure the link is healthy */
			p_physp = osm_node_get_physp_ptr(p_sw->p_node, port);
			if (!p_physp || !osm_link_is_healthy(p_physp))
				continue;
			/* we don't add upstream ports in this step */
			if (osm_node_get_type(remote_node) != IB_NODE_TYPE_CA)
				continue;

			guid = osm_physp_get_port_guid(osm_node_get_physp_ptr(
						       remote_node,
						       remote_port));
			if (cl_qmap_get(port_map, guid)
			    != cl_qmap_end(port_map))
				osm_mcast_tbl_set(p_tbl, mlid_ho, port);
		}

		/* now we have to add the upstream port of 'this' switch and
		   the downstream port of the next switch to the mcast table
		   until we reach the root_sw
		 */
		curr_adj = &adj_list[i];
		while (curr_adj->sw != root_sw) {
			/* the used_link is the link that was used in dijkstra to reach this node,
			   so the to_port is the local (upstream) port on curr_adj->sw
			 */
			upstream_port = curr_adj->used_link->to_port;
			osm_mcast_tbl_set(p_tbl, mlid_ho, upstream_port);

			/* now we go one step in direction root_sw and add the
			   downstream port for the spanning tree
			 */
			downstream_port = curr_adj->used_link->from_port;
			p_tbl = osm_switch_get_mcast_tbl_ptr(
				adj_list[curr_adj->used_link->from].sw);
			osm_mcast_tbl_set(p_tbl, mlid_ho, downstream_port);

			curr_adj = &adj_list[curr_adj->used_link->from];
		}
	}

	OSM_LOG_EXIT(p_sm->p_log);
	return 0;
}