static ib_api_status_t
osmtest_write_all_link_recs(IN osmtest_t * const p_osmt, IN FILE * fh)
{
	osmtest_req_context_t context;
	const ib_link_record_t *p_rec;
	uint32_t i;
	cl_status_t status;
	size_t num_recs;
	int result;

	OSM_LOG_ENTER(&p_osmt->log);

	memset(&context, 0, sizeof(context));

	/*
	 * Do a blocking query for all NodeRecords in the subnet.
	 */
	status = osmtest_get_all_recs(p_osmt, IB_MAD_ATTR_LINK_RECORD,
				      sizeof(*p_rec), &context);

	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0165: "
			"osmtest_get_all_recs failed (%s)\n",
			ib_get_err_str(status));
		goto Exit;
	}

	/*
	 * Write the received records out to the file.
	 */
	num_recs = context.result.result_cnt;

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
		"Received %zu records\n", num_recs);

	result = fprintf(fh, "#\n" "# Link Records\n" "#\n");
	if (result < 0) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0166: "
			"Write failed\n");
		status = IB_ERROR;
		goto Exit;
	}

	for (i = 0; i < num_recs; i++) {
		p_rec =
		    (ib_link_record_t *) osmv_get_query_result(context.result.
							       p_result_madw,
							       i);

		osmtest_write_link(p_osmt, fh, p_rec);
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