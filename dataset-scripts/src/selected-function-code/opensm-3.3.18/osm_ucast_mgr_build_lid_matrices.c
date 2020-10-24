int osm_ucast_mgr_build_lid_matrices(IN osm_ucast_mgr_t * p_mgr)
{
	uint32_t i;
	uint32_t iteration_max;
	cl_qmap_t *p_sw_guid_tbl;

	p_sw_guid_tbl = &p_mgr->p_subn->sw_guid_tbl;

	OSM_LOG(p_mgr->p_log, OSM_LOG_VERBOSE,
		"Starting switches' Min Hop Table Assignment\n");

	/*
	   Set up the weighting factors for the routing.
	 */
	cl_qmap_apply_func(p_sw_guid_tbl, set_default_hop_wf, NULL);
	if (p_mgr->p_subn->opt.hop_weights_file) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Fetching hop weight factor file \'%s\'\n",
			p_mgr->p_subn->opt.hop_weights_file);
		if (parse_node_map(p_mgr->p_subn->opt.hop_weights_file,
				   set_hop_wf, p_mgr)) {
			OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR, "ERR 3A05: "
				"cannot parse hop_weights_file \'%s\'\n",
				p_mgr->p_subn->opt.hop_weights_file);
		}
	}

	/*
	   Set the switch matrices for each switch's own port 0 LID(s)
	   then set the lid matrices for the each switch's leaf nodes.
	 */
	cl_qmap_apply_func(p_sw_guid_tbl, ucast_mgr_process_hop_0_1, p_mgr);

	/*
	   Get the switch matrices for each switch's neighbors.
	   This process requires a number of iterations equal to
	   the number of switches in the subnet minus 1.

	   In each iteration, a switch learns the lid/port/hop
	   information (as contained by a switch's lid matrix) from
	   its immediate neighbors.  After each iteration, a switch
	   (and it's neighbors) know more routing information than
	   it did on the previous iteration.
	   Thus, by repeatedly absorbing the routing information of
	   neighbor switches, every switch eventually learns how to
	   route all LIDs on the subnet.

	   Note that there may not be any switches in the subnet if
	   we are in simple p2p configuration.
	 */
	iteration_max = cl_qmap_count(p_sw_guid_tbl);

	/*
	   If there are switches in the subnet, iterate until the lid
	   matrix has been constructed.  Otherwise, just immediately
	   indicate we're done if no switches exist.
	 */
	if (iteration_max) {
		iteration_max--;

		/*
		   we need to find out when the propagation of
		   hop counts has relaxed. So this global variable
		   is preset to 0 on each iteration and if
		   if non of the switches was set will exit the
		   while loop
		 */
		p_mgr->some_hop_count_set = TRUE;
		for (i = 0; (i < iteration_max) && p_mgr->some_hop_count_set;
		     i++) {
			p_mgr->some_hop_count_set = FALSE;
			cl_qmap_apply_func(p_sw_guid_tbl,
					   ucast_mgr_process_neighbors, p_mgr);
		}
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Min-hop propagated in %d steps\n", i);
	}

	return 0;
}