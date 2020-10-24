static void ni_rcv_set_links(IN osm_sm_t * sm, osm_node_t * p_node,
			     const uint8_t port_num,
			     const osm_ni_context_t * p_ni_context)
{
	osm_node_t *p_neighbor_node;
	osm_physp_t *p_physp, *p_remote_physp;

	OSM_LOG_ENTER(sm->p_log);

	/*
	   A special case exists in which the node we're trying to
	   link is our own node.  In this case, the guid value in
	   the ni_context will be zero.
	 */
	if (p_ni_context->node_guid == 0) {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Nothing to link for our own node 0x%" PRIx64 "\n",
			cl_ntoh64(osm_node_get_node_guid(p_node)));
		goto _exit;
	}

	p_neighbor_node = osm_get_node_by_guid(sm->p_subn,
					       p_ni_context->node_guid);
	if (PF(!p_neighbor_node)) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D10: "
			"Unexpected removal of neighbor node 0x%" PRIx64 "\n",
			cl_ntoh64(p_ni_context->node_guid));
		goto _exit;
	}

	/* When setting the link, ports on both
	   sides of the link should be initialized */
	CL_ASSERT(osm_node_link_has_valid_ports(p_node, port_num,
						p_neighbor_node,
						p_ni_context->port_num));

	if (osm_node_link_exists(p_node, port_num,
				 p_neighbor_node, p_ni_context->port_num)) {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG, "Link already exists\n");
		goto _exit;
	}

	p_physp = osm_node_get_physp_ptr(p_node, port_num);
	/*
	 * If the link went UP, after we already discovered it, we shouldn't
	 * set the link between the ports and resweep.
	 */
	if (osm_physp_get_port_state(p_physp) == IB_LINK_DOWN &&
	    p_node->physp_discovered[port_num]) {
		/* Link down on another side. Don't create a link*/
		p_node->physp_discovered[port_num] = 0;
		sm->p_subn->force_heavy_sweep = TRUE;
		goto _exit;
	}

	if (osm_node_has_any_link(p_node, port_num) &&
	    sm->p_subn->force_heavy_sweep == FALSE &&
	    (!p_ni_context->dup_count ||
	     (p_ni_context->dup_node_guid == osm_node_get_node_guid(p_node) &&
	      p_ni_context->dup_port_num == port_num))) {
		/*
		   Uh oh...
		   This could be reconnected ports, but also duplicated GUID
		   (2 nodes have the same guid) or a 12x link with lane reversal
		   that is not configured correctly.
		   We will try to recover by querying NodeInfo again.
		   In order to catch even fast port moving to new location(s)
		   and back we will count up to 5.
		   Some crazy reconnections (newly created switch loop right
		   before targeted CA) will not be catched this way. So in worst
		   case - report GUID duplication and request new discovery.
		   When switch node is targeted NodeInfo querying will be done
		   in opposite order, this is much stronger check, unfortunately
		   it is impossible with CAs.
		 */
		p_physp = osm_node_get_physp_ptr(p_node, port_num);
		if (p_ni_context->dup_count > 5) {
			report_duplicated_guid(sm, p_physp, p_neighbor_node,
					       p_ni_context->port_num);
			sm->p_subn->force_heavy_sweep = TRUE;
		} else if (p_node->sw)
			requery_dup_node_info(sm, p_physp->p_remote_physp,
					      p_ni_context->dup_count + 1);
		else
			requery_dup_node_info(sm, p_physp,
					      p_ni_context->dup_count + 1);
	}

	/*
	   When there are only two nodes with exact same guids (connected back
	   to back) - the previous check for duplicated guid will not catch
	   them. But the link will be from the port to itself...
	   Enhanced Port 0 is an exception to this
	 */
	if (osm_node_get_node_guid(p_node) == p_ni_context->node_guid &&
	    port_num == p_ni_context->port_num &&
	    port_num != 0 && cl_qmap_count(&sm->p_subn->sw_guid_tbl) == 0) {
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Duplicate GUID found by link from a port to itself:"
			"node 0x%" PRIx64 ", port number %u\n",
			cl_ntoh64(osm_node_get_node_guid(p_node)), port_num);
		p_physp = osm_node_get_physp_ptr(p_node, port_num);
		osm_dump_dr_path_v2(sm->p_log, osm_physp_get_dr_path_ptr(p_physp),
				    FILE_ID, OSM_LOG_VERBOSE);

		if (sm->p_subn->opt.exit_on_fatal == TRUE) {
			osm_log_v2(sm->p_log, OSM_LOG_SYS, FILE_ID,
				   "Errors on subnet. Duplicate GUID found "
				   "by link from a port to itself. "
				   "See verbose opensm.log for more details\n");
			exit(1);
		}
	}

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
		"Creating new link between:\n\t\t\t\tnode 0x%" PRIx64
		", port number %u and\n\t\t\t\tnode 0x%" PRIx64
		", port number %u\n",
		cl_ntoh64(osm_node_get_node_guid(p_node)), port_num,
		cl_ntoh64(p_ni_context->node_guid), p_ni_context->port_num);

	if (sm->ucast_mgr.cache_valid)
		osm_ucast_cache_check_new_link(&sm->ucast_mgr, p_node, port_num,
					       p_neighbor_node,
					       p_ni_context->port_num);

	osm_node_link(p_node, port_num, p_neighbor_node,
		      p_ni_context->port_num);

	p_physp = osm_node_get_physp_ptr(p_node, port_num);
	p_remote_physp = osm_node_get_physp_ptr(p_neighbor_node,
						p_ni_context->port_num);
	osm_db_neighbor_set(sm->p_subn->p_neighbor,
			    cl_ntoh64(osm_physp_get_port_guid(p_physp)),
			    port_num,
			    cl_ntoh64(osm_physp_get_port_guid(p_remote_physp)),
			    p_ni_context->port_num);
	osm_db_neighbor_set(sm->p_subn->p_neighbor,
			    cl_ntoh64(osm_physp_get_port_guid(p_remote_physp)),
			    p_ni_context->port_num,
			    cl_ntoh64(osm_physp_get_port_guid(p_physp)),
			    port_num);

_exit:
	OSM_LOG_EXIT(sm->p_log);
}