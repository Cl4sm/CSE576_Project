static ib_api_status_t
osmtest_write_all_node_recs(IN osmtest_t * const p_osmt, IN FILE * fh)
{
	osmtest_req_context_t context;
	node_t *p_node;
	node_t *p_guid_node;
	const ib_node_record_t *p_rec;
	cl_status_t status = CL_SUCCESS;
	int result;
	uint16_t lid;

	OSM_LOG_ENTER(&p_osmt->log);

	result = fprintf(fh, "#\n" "# Node Records\n" "#\n");
	if (result < 0) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0027: "
			"Write failed\n");
		status = IB_ERROR;
		goto Exit;
	}

	/*
	 * Go over all LIDs in the range 1 to max_lid and do a
	 * NodeRecord query by that lid.
	 */
	for (lid = 1; lid <= p_osmt->max_lid; lid++) {
		/* prepare the query context */
		memset(&context, 0, sizeof(context));

		status =
		    osmtest_get_node_rec_by_lid(p_osmt, cl_ntoh16(lid),
						&context);
		if (status != IB_SUCCESS) {
			if (status != IB_SA_MAD_STATUS_NO_RECORDS) {
				OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG, "ERR 0028: "
					"failed to get node info for LID:0x%02X (%s)\n",
					cl_ntoh16(lid), ib_get_err_str(status));
				goto Exit;
			} else {
				OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG, "WRN 0121: "
					"failed to get node info for LID:0x%02X (%s)\n",
					cl_ntoh16(lid), ib_get_err_str(status));
				status = IB_SUCCESS;
			}
		} else {
			/* OK we got something */
			p_rec =
			    osmv_get_query_node_rec(context.result.
						    p_result_madw, 0);
			osmtest_write_node_info(p_osmt, fh, p_rec);

			/* create a subnet object */
			p_node = node_new();
			CL_ASSERT(p_node != NULL);

			/* copy the info to the subnet node object */
			p_node->rec = *p_rec;

			cl_qmap_insert(&p_osmt->exp_subn.node_lid_tbl,
				       p_node->rec.lid, &p_node->map_item);

			p_guid_node = node_new();
			CL_ASSERT(p_guid_node != NULL);

			*p_guid_node = *p_node;

			cl_qmap_insert(&p_osmt->exp_subn.node_guid_tbl,
				       p_guid_node->rec.node_info.node_guid,
				       &p_guid_node->map_item);

		}

		if (context.result.p_result_madw != NULL) {
			osm_mad_pool_put(&p_osmt->mad_pool,
					 context.result.p_result_madw);
			context.result.p_result_madw = NULL;
		}
	}

Exit:
	if (context.result.p_result_madw != NULL) {
		osm_mad_pool_put(&p_osmt->mad_pool,
				 context.result.p_result_madw);
		context.result.p_result_madw = NULL;
	}

	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}