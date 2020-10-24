void osm_ucast_cache_add_link(osm_ucast_mgr_t * p_mgr,
			      osm_physp_t * p_physp1, osm_physp_t * p_physp2)
{
	osm_node_t *p_node_1 = p_physp1->p_node, *p_node_2 = p_physp2->p_node;
	uint16_t lid_ho_1, lid_ho_2;

	OSM_LOG_ENTER(p_mgr->p_log);

	if (!p_mgr->cache_valid)
		goto Exit;

	if (osm_node_get_type(p_node_1) != IB_NODE_TYPE_SWITCH &&
	    osm_node_get_type(p_node_2) != IB_NODE_TYPE_SWITCH) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG, "Dropping CA-2-CA link\n");
		osm_ucast_cache_invalidate(p_mgr);
		goto Exit;
	}

	if ((osm_node_get_type(p_node_1) == IB_NODE_TYPE_SWITCH &&
	     !osm_node_get_physp_ptr(p_node_1, 0)) ||
	    (osm_node_get_type(p_node_2) == IB_NODE_TYPE_SWITCH &&
	     !osm_node_get_physp_ptr(p_node_2, 0))) {
		/* we're caching a link when one of the nodes
		   has already been dropped and cached */
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Port %u <-> port %u: port0 on one of the nodes "
			"has already been dropped and cached\n",
			p_physp1->port_num, p_physp2->port_num);
		goto Exit;
	}

	/* One of the nodes is switch. Just for code
	   simplicity, make sure that it's the first node. */

	if (osm_node_get_type(p_node_1) != IB_NODE_TYPE_SWITCH) {
		osm_physp_t *tmp = p_physp1;
		p_physp1 = p_physp2;
		p_physp2 = tmp;
		p_node_1 = p_physp1->p_node;
		p_node_2 = p_physp2->p_node;
	}

	if (!p_node_1->sw) {
		/* something is wrong - we'd better not use cache */
		osm_ucast_cache_invalidate(p_mgr);
		goto Exit;
	}

	lid_ho_1 = cl_ntoh16(osm_node_get_base_lid(p_node_1, 0));

	if (osm_node_get_type(p_node_2) == IB_NODE_TYPE_SWITCH) {

		if (!p_node_2->sw) {
			/* something is wrong - we'd better not use cache */
			osm_ucast_cache_invalidate(p_mgr);
			goto Exit;
		}

		lid_ho_2 = cl_ntoh16(osm_node_get_base_lid(p_node_2, 0));

		/* lost switch-2-switch link - cache both sides */
		cache_add_sw_link(p_mgr, p_physp1, lid_ho_2, FALSE);
		cache_add_sw_link(p_mgr, p_physp2, lid_ho_1, FALSE);
	} else {
		lid_ho_2 = cl_ntoh16(osm_physp_get_base_lid(p_physp2));

		/* lost link to CA/RTR - cache only switch side */
		cache_add_sw_link(p_mgr, p_physp1, lid_ho_2, TRUE);
	}

Exit:
	OSM_LOG_EXIT(p_mgr->p_log);
}				/* osm_ucast_cache_add_link() */