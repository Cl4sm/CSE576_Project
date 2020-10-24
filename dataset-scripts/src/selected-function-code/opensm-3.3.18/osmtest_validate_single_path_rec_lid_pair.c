static ib_api_status_t
osmtest_validate_single_path_rec_lid_pair(IN osmtest_t * const p_osmt,
					  IN path_t * const p_path)
{
	osmtest_req_context_t context;
	const ib_path_rec_t *p_rec;
	cl_status_t status = IB_SUCCESS;
	size_t num_recs;

	OSM_LOG_ENTER(&p_osmt->log);

	memset(&context, 0, sizeof(context));

	status = osmtest_get_path_rec_by_lid_pair(p_osmt,
						  p_path->rec.slid,
						  p_path->rec.dlid, &context);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0102: "
			"ib_query failed (%s)\n", ib_get_err_str(status));
		goto Exit;
	}

	num_recs = context.result.result_cnt;
	if (num_recs != 1) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0103: "
			"Too many records. Expected 1, received %zu\n",
			num_recs);

		status = IB_ERROR;
	} else {
		p_rec =
		    osmv_get_query_path_rec(context.result.p_result_madw, 0);

		status = osmtest_validate_path_data(p_osmt, p_path, p_rec);
		if (status != IB_SUCCESS) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0104: "
				"osmtest_validate_path_data failed (%s)\n",
				ib_get_err_str(status));
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