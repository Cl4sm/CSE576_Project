static ib_api_status_t
osmtest_validate_single_port_recs(IN osmtest_t * const p_osmt)
{
	port_t *p_port;
	cl_status_t status = IB_SUCCESS;
	const cl_qmap_t *p_port_key_tbl;
	uint16_t cnt = 0;

	OSM_LOG_ENTER(&p_osmt->log);

	p_port_key_tbl = &p_osmt->exp_subn.port_key_tbl;

	osmtest_prepare_db(p_osmt);

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
		"Validating individual port record queries\n");

	/*
	 * Walk the list of all port records, and ask for each one
	 * specifically.  Make sure we get it.
	 */
	p_port = (port_t *) cl_qmap_head(p_port_key_tbl);
	while (p_port != (port_t *) cl_qmap_end(p_port_key_tbl)) {
		status = osmtest_validate_single_port_rec_lid(p_osmt, p_port);
		if (status != IB_SUCCESS) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 011B: "
				"osmtest_validate_single_port_rec_lid (%s)\n",
				ib_get_err_str(status));
			goto Exit;
		}
		cnt++;
		p_port = (port_t *) cl_qmap_next(&p_port->map_item);
	}

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
		"Total of %u port records validated\n", cnt);

	status = osmtest_check_missing_ports(p_osmt);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0118: "
			"osmtest_check_missing_paths failed (%s)\n",
			ib_get_err_str(status));
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}