ib_api_status_t
osmtest_stress_path_recs_by_lid(IN osmtest_t * const p_osmt,
				OUT uint32_t * const p_num_recs,
				OUT uint32_t * const p_num_queries)
{
	osmtest_req_context_t context;
	ib_path_rec_t *p_rec;
	cl_status_t status;
	ib_net16_t dlid, slid;
	int num_recs, i;

	OSM_LOG_ENTER(&p_osmt->log);

	memset(&context, 0, sizeof(context));

	slid = cl_ntoh16(p_osmt->local_port.lid);
	dlid = cl_ntoh16(p_osmt->local_port.sm_lid);

	/*
	 * Do a blocking query for the PathRecord.
	 */
	status = osmtest_get_path_rec_by_lid_pair(p_osmt, slid, dlid, &context);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 000A: "
			"osmtest_get_path_rec_by_lid_pair failed (%s)\n",
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
			p_rec = osmv_get_query_path_rec(context.result.p_result_madw, 0);
			osm_dump_path_record(&p_osmt->log, p_rec, OSM_LOG_VERBOSE);
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