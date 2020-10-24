ib_api_status_t
osmtest_get_local_port_lmc(IN osmtest_t * const p_osmt,
			   IN ib_net16_t lid, OUT uint8_t * const p_lmc)
{
	osmtest_req_context_t context;
	ib_portinfo_record_t *p_rec;
	uint32_t i;
	cl_status_t status;
	uint32_t num_recs = 0;

	OSM_LOG_ENTER(&p_osmt->log);

	memset(&context, 0, sizeof(context));

	/*
	 * Do a blocking query for our own PortInfoRecord in the subnet.
	 */
	status = osmtest_get_port_rec(p_osmt, cl_ntoh16(lid), &context);

	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 001A: "
			"osmtest_get_port_rec failed (%s)\n",
			ib_get_err_str(status));
		goto Exit;
	}

	num_recs = context.result.result_cnt;

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "Received %u records\n", num_recs);

	for (i = 0; i < num_recs; i++) {
		p_rec =
		    osmv_get_query_portinfo_rec(context.result.p_result_madw,
						i);
		osm_dump_portinfo_record(&p_osmt->log, p_rec, OSM_LOG_VERBOSE);
		if (p_lmc) {
			*p_lmc = ib_port_info_get_lmc(&p_rec->port_info);
			OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG, "LMC %d\n", *p_lmc);
		}
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