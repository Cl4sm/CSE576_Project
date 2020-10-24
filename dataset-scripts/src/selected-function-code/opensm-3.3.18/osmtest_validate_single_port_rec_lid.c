static ib_api_status_t
osmtest_validate_single_port_rec_lid(IN osmtest_t * const p_osmt,
				     IN port_t * const p_port)
{
	osmtest_req_context_t context;

	const ib_portinfo_record_t *p_rec;
	cl_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(&p_osmt->log);

	memset(&context, 0, sizeof(context));

	context.p_osmt = p_osmt;
	status = osmtest_get_port_rec_by_num(p_osmt,
					     p_port->rec.lid,
					     p_port->rec.port_num, &context);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0108: "
			"ib_query failed (%s)\n", ib_get_err_str(status));

		goto Exit;
	}

	/* we should have got exactly one port */
	p_rec = osmv_get_query_portinfo_rec(context.result.p_result_madw, 0);
	status = osmtest_validate_port_rec(p_osmt, p_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0109: "
			"osmtest_validate_port_data failed (%s)\n",
			ib_get_err_str(status));
	}

Exit:
	/*
	 * Return the IB query MAD to the pool as necessary.
	 */
	if (context.result.p_result_madw != NULL) {
		osm_mad_pool_put(&p_osmt->mad_pool,
				 context.result.p_result_madw);
		context.result.p_result_madw = NULL;
	}

	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}