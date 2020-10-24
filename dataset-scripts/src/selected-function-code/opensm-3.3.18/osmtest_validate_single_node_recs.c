static ib_api_status_t
osmtest_validate_single_node_recs(IN osmtest_t * const p_osmt)
{
	node_t *p_node;
	cl_status_t status = IB_SUCCESS;
	const cl_qmap_t *p_node_lid_tbl;
	uint16_t cnt = 0;

	OSM_LOG_ENTER(&p_osmt->log);

	p_node_lid_tbl = &p_osmt->exp_subn.node_lid_tbl;

	osmtest_prepare_db(p_osmt);

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
		"Validating individual node record queries\n");

	/*
	 * Walk the list of all node records, and ask for each one
	 * specifically.  Make sure we get it.
	 */
	p_node = (node_t *) cl_qmap_head(p_node_lid_tbl);
	while (p_node != (node_t *) cl_qmap_end(p_node_lid_tbl)) {
		status = osmtest_validate_single_node_rec_lid(p_osmt,
							      (ib_net16_t)
							      cl_qmap_key((cl_map_item_t *) p_node), p_node);
		if (status != IB_SUCCESS) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 011A: "
				"osmtest_validate_single_node_rec_lid (%s)\n",
				ib_get_err_str(status));
			goto Exit;
		}
		cnt++;
		p_node = (node_t *) cl_qmap_next(&p_node->map_item);
	}

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
		"Total of %u node records validated\n", cnt);

	status = osmtest_check_missing_nodes(p_osmt);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0117: "
			"osmtest_check_missing_nodes (%s)\n",
			ib_get_err_str(status));
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}