static ib_api_status_t mcast_mgr_build_spanning_tree(osm_sm_t * sm,
						     osm_mgrp_box_t * mbox)
{
	cl_qlist_t port_list;
	cl_qmap_t port_map;
	uint32_t num_ports;
	osm_switch_t *p_sw;
	ib_api_status_t status = IB_SUCCESS;
	uint8_t max_depth = 0;

	OSM_LOG_ENTER(sm->p_log);

	/*
	   TO DO - for now, just blow away the old tree.
	   In the future we'll need to construct the tree based
	   on multicast forwarding table information if the user wants to
	   preserve existing multicast routes.
	 */
	osm_purge_mtree(sm, mbox);

	/* build the first "subset" containing all member ports */
	if (osm_mcast_make_port_list_and_map(&port_list, &port_map, mbox)) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0A10: "
			"Insufficient memory to make port list\n");
		status = IB_ERROR;
		goto Exit;
	}

	num_ports = cl_qlist_count(&port_list);
	if (num_ports < 2) {
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"MLID 0x%X has %u members - nothing to do\n",
			mbox->mlid, num_ports);
		osm_mcast_drop_port_list(&port_list);
		goto Exit;
	}

	/*
	   This function builds the single spanning tree recursively.
	   At each stage, the ports to be reached are divided into
	   non-overlapping subsets of member ports that can be reached through
	   a given switch port.  Construction then moves down each
	   branch, and the process starts again with each branch computing
	   for its own subset of the member ports.

	   The maximum recursion depth is at worst the maximum hop count in the
	   subnet, which is spec limited to 64.
	 */

	/*
	   Locate the switch around which to create the spanning
	   tree for this multicast group.
	 */
	p_sw = osm_mcast_mgr_find_root_switch(sm, &port_list);
	if (p_sw == NULL) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0A08: "
			"Unable to locate a suitable switch for group 0x%X\n",
			mbox->mlid);
		osm_mcast_drop_port_list(&port_list);
		status = IB_ERROR;
		goto Exit;
	}

	mbox->root = mcast_mgr_branch(sm, mbox->mlid, p_sw, &port_list, 0, 0,
				      &max_depth);

	OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
		"Configured MLID 0x%X for %u ports, max tree depth = %u\n",
		mbox->mlid, num_ports, max_depth);
Exit:
	OSM_LOG_EXIT(sm->p_log);
	return status;
}