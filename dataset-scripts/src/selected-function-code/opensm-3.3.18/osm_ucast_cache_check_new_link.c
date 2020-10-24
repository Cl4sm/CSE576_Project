void osm_ucast_cache_check_new_link(osm_ucast_mgr_t * p_mgr,
				    osm_node_t * p_node_1, uint8_t port_num_1,
				    osm_node_t * p_node_2, uint8_t port_num_2)
{
	uint16_t lid_ho_1;
	uint16_t lid_ho_2;

	OSM_LOG_ENTER(p_mgr->p_log);

	if (!p_mgr->cache_valid)
		goto Exit;

	cache_check_link_change(p_mgr,
				osm_node_get_physp_ptr(p_node_1, port_num_1),
				osm_node_get_physp_ptr(p_node_2, port_num_2));

	if (!p_mgr->cache_valid)
		goto Exit;

	if (osm_node_get_type(p_node_1) != IB_NODE_TYPE_SWITCH &&
	    osm_node_get_type(p_node_2) != IB_NODE_TYPE_SWITCH) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG, "Found CA-2-CA link\n");
		osm_ucast_cache_invalidate(p_mgr);
		goto Exit;
	}

	/* for code simplicity, we want the first node to be switch */
	if (osm_node_get_type(p_node_1) != IB_NODE_TYPE_SWITCH) {
		osm_node_t *tmp_node = p_node_1;
		uint8_t tmp_port_num = port_num_1;
		p_node_1 = p_node_2;
		port_num_1 = port_num_2;
		p_node_2 = tmp_node;
		port_num_2 = tmp_port_num;
	}

	lid_ho_1 = cl_ntoh16(osm_node_get_base_lid(p_node_1, 0));

	if (osm_node_get_type(p_node_2) == IB_NODE_TYPE_SWITCH)
		lid_ho_2 = cl_ntoh16(osm_node_get_base_lid(p_node_2, 0));
	else
		lid_ho_2 =
		    cl_ntoh16(osm_node_get_base_lid(p_node_2, port_num_2));

	if (!lid_ho_1 || !lid_ho_2) {
		/*
		 * No lid assigned, which means that one of the nodes is new.
		 * Need to wait for lid manager to process this node.
		 * The switches and their links will be checked later when
		 * the whole cache validity will be verified.
		 */
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Link port %u <-> %u reveals new node - cache will "
			"be validated later\n", port_num_1, port_num_2);
		goto Exit;
	}

	cache_remove_port(p_mgr, lid_ho_1, port_num_1, lid_ho_2,
			  (osm_node_get_type(p_node_2) !=
			  IB_NODE_TYPE_SWITCH));

	/* if node_2 is a switch, the link should be cleaned from its cache */

	if (osm_node_get_type(p_node_2) == IB_NODE_TYPE_SWITCH)
		cache_remove_port(p_mgr, lid_ho_2,
				  port_num_2, lid_ho_1, FALSE);

Exit:
	OSM_LOG_EXIT(p_mgr->p_log);
}				/* osm_ucast_cache_check_new_link() */