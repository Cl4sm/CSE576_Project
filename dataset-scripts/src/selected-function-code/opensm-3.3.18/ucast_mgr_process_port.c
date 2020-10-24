static void ucast_mgr_process_port(IN osm_ucast_mgr_t * p_mgr,
				   IN osm_switch_t * p_sw,
				   IN osm_port_t * p_port,
				   IN unsigned lid_offset)
{
	uint16_t min_lid_ho;
	uint16_t max_lid_ho;
	uint16_t lid_ho;
	uint8_t port;
	boolean_t is_ignored_by_port_prof;
	ib_net64_t node_guid;
	unsigned start_from = 1;

	OSM_LOG_ENTER(p_mgr->p_log);

	osm_port_get_lid_range_ho(p_port, &min_lid_ho, &max_lid_ho);

	/* If the lids are zero - then there was some problem with
	 * the initialization. Don't handle this port. */
	if (min_lid_ho == 0 || max_lid_ho == 0) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR, "ERR 3A04: "
			"Port 0x%"PRIx64" (%s port %d) has LID 0. An "
			"initialization error occurred. Ignoring port\n",
			cl_ntoh64(osm_port_get_guid(p_port)),
			p_port->p_node->print_desc,
			p_port->p_physp->port_num);
		goto Exit;
	}

	lid_ho = min_lid_ho + lid_offset;

	if (lid_ho > max_lid_ho)
		goto Exit;

	if (lid_offset && !p_mgr->is_dor)
		/* ignore potential overflow - it is handled in osm_switch.c */
		start_from = osm_switch_get_port_by_lid(p_sw, lid_ho - 1) + 1;

	OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
		"Processing port 0x%" PRIx64
		" (\'%s\' port %u), LID %u [%u,%u]\n",
		cl_ntoh64(osm_port_get_guid(p_port)),
		p_port->p_node->print_desc, p_port->p_physp->port_num, lid_ho,
		min_lid_ho, max_lid_ho);

	/* TODO - This should be runtime error, not a CL_ASSERT() */
	CL_ASSERT(max_lid_ho <= IB_LID_UCAST_END_HO);

	node_guid = osm_node_get_node_guid(p_sw->p_node);

	/*
	   The lid matrix contains the number of hops to each
	   lid from each port.  From this information we determine
	   how best to distribute the LID range across the ports
	   that can reach those LIDs.
	 */
	port = osm_switch_recommend_path(p_sw, p_port, lid_ho, start_from,
					 p_mgr->p_subn->ignore_existing_lfts,
					 p_mgr->p_subn->opt.lmc,
					 p_mgr->is_dor,
					 p_mgr->p_subn->opt.port_shifting,
					 p_port->use_scatter);

	if (port == OSM_NO_PATH) {
		/* do not try to overwrite the ppro of non existing port ... */
		is_ignored_by_port_prof = TRUE;

		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"No path to get to LID %u from switch 0x%" PRIx64 "\n",
			lid_ho, cl_ntoh64(node_guid));
	} else {
		osm_physp_t *p = osm_node_get_physp_ptr(p_sw->p_node, port);

		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Routing LID %u to port %u for switch 0x%" PRIx64 "\n",
			lid_ho, port, cl_ntoh64(node_guid));

		/*
		   we would like to optionally ignore this port in equalization
		   as in the case of the Mellanox Anafa Internal PCI TCA port
		 */
		is_ignored_by_port_prof = p->is_prof_ignored;

		/*
		   We also would ignore this route if the target lid is of
		   a switch and the port_profile_switch_node is not TRUE
		 */
		if (!p_mgr->p_subn->opt.port_profile_switch_nodes)
			is_ignored_by_port_prof |=
			    (osm_node_get_type(p_port->p_node) ==
			     IB_NODE_TYPE_SWITCH);
	}

	/*
	   We have selected the port for this LID.
	   Write it to the forwarding tables.
	 */
	p_sw->new_lft[lid_ho] = port;
	if (!is_ignored_by_port_prof) {
		struct osm_remote_node *rem_node_used;
		osm_switch_count_path(p_sw, port);
		if (port > 0 && p_port->priv &&
		    (rem_node_used = find_and_add_remote_sys(p_sw, port,
							     p_mgr->is_dor,
							     p_port->priv)))
			rem_node_used->forwarded_to++;
	}

Exit:
	OSM_LOG_EXIT(p_mgr->p_log);
}