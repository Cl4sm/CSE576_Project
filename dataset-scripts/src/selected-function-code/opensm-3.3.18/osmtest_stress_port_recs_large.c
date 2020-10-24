ib_api_status_t
osmtest_stress_port_recs_large(IN osmtest_t * const p_osmt,
			       OUT uint32_t * const p_num_recs,
			       OUT uint32_t * const p_num_queries)
{
	osmtest_req_context_t context;
	ib_portinfo_record_t *p_rec;
	uint32_t i;
	cl_status_t status;
	uint32_t num_recs = 0;

	OSM_LOG_ENTER(&p_osmt->log);

	memset(&context, 0, sizeof(context));
	/*
	 * Do a blocking query for all PortInfoRecords in the subnet.
	 */
	status = osmtest_get_all_recs(p_osmt, IB_MAD_ATTR_PORTINFO_RECORD,
				      sizeof(*p_rec), &context);

	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0006: "
			"osmtest_get_all_recs failed (%s)\n",
			ib_get_err_str(status));
		goto Exit;
	}

	/*
	 * Populate the database with the received records.
	 */
	num_recs = context.result.result_cnt;
	*p_num_recs += num_recs;
	++*p_num_queries;

	if (osm_log_is_active(&p_osmt->log, OSM_LOG_VERBOSE)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
			"Received %u records\n", num_recs);

		for (i = 0; i < num_recs; i++) {
			p_rec =
			    osmv_get_query_portinfo_rec(context.result.
							p_result_madw, i);
			osm_dump_portinfo_record(&p_osmt->log, p_rec,
						 OSM_LOG_VERBOSE);
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