static void sir_rcv_create_sir(IN osm_sa_t * sa, IN const osm_switch_t * p_sw,
			       IN cl_qlist_t * p_list, IN ib_net16_t match_lid,
			       IN const osm_physp_t * p_req_physp)
{
	osm_port_t *p_port;
	const osm_physp_t *p_physp;
	uint16_t match_lid_ho;
	ib_net16_t min_lid_ho;
	ib_net16_t max_lid_ho;

	OSM_LOG_ENTER(sa->p_log);

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"Looking for SwitchInfoRecord with LID: %u\n",
		cl_ntoh16(match_lid));

	/* In switches, the port guid is the node guid. */
	p_port =
	    osm_get_port_by_guid(sa->p_subn, p_sw->p_node->node_info.port_guid);
	if (!p_port) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 530A: "
			"Failed to find Port by Node Guid:0x%016" PRIx64
			"\n", cl_ntoh64(p_sw->p_node->node_info.node_guid));
		goto Exit;
	}

	/* check that the requester physp and the current physp are under
	   the same partition. */
	p_physp = p_port->p_physp;
	if (!p_physp) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 530B: "
			"Failed to find default physical Port by Node Guid:0x%016"
			PRIx64 "\n",
			cl_ntoh64(p_sw->p_node->node_info.node_guid));
		goto Exit;
	}
	if (!osm_physp_share_pkey(sa->p_log, p_req_physp, p_physp,
				  sa->p_subn->opt.allow_both_pkeys))
		goto Exit;

	/* get the port 0 of the switch */
	osm_port_get_lid_range_ho(p_port, &min_lid_ho, &max_lid_ho);

	match_lid_ho = cl_ntoh16(match_lid);
	if (match_lid_ho) {
		/*
		   We validate that the lid belongs to this switch.
		 */
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Comparing LID: %u <= %u <= %u\n",
			min_lid_ho, match_lid_ho, max_lid_ho);

		if (match_lid_ho < min_lid_ho || match_lid_ho > max_lid_ho)
			goto Exit;

	}

	sir_rcv_new_sir(sa, p_sw, p_list, osm_port_get_base_lid(p_port));

Exit:
	OSM_LOG_EXIT(sa->p_log);
}