ib_api_status_t
osmtest_stress_path_recs_by_guid(IN osmtest_t * const p_osmt,
				 OUT uint32_t * const p_num_recs,
				 OUT uint32_t * const p_num_queries)
{
	osmtest_req_context_t context;
	ib_path_rec_t *p_rec;
	uint32_t i;
	cl_status_t status = IB_SUCCESS;
	uint32_t num_recs = 0;
	node_t *p_src_node, *p_dst_node;
	cl_qmap_t *p_tbl;

	OSM_LOG_ENTER(&p_osmt->log);

	memset(&context, 0, sizeof(context));

	context.p_osmt = p_osmt;

	p_tbl = &p_osmt->exp_subn.node_guid_tbl;

	p_src_node = (node_t *) cl_qmap_head(p_tbl);

	/*
	 * Go over all nodes that exist in the subnet
	 * for each pair that are not switch nodes get the path record
	 */
	while (p_src_node != (node_t *) cl_qmap_end(p_tbl)) {
		p_dst_node = (node_t *) cl_qmap_head(p_tbl);

		while (p_dst_node != (node_t *) cl_qmap_end(p_tbl)) {
			/*
			 * Do a blocking query for CA to CA Path Record
			 */
			OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
				"Source : guid = 0x%" PRIx64 " type = %d"
				"Target : guid = 0x%" PRIx64 " type = %d\n",
				cl_ntoh64(p_src_node->rec.node_info.port_guid),
				p_src_node->rec.node_info.node_type,
				cl_ntoh64(p_dst_node->rec.node_info.port_guid),
				p_dst_node->rec.node_info.node_type);

			if (p_src_node->rec.node_info.node_type ==
			    IB_NODE_TYPE_CA
			    && p_dst_node->rec.node_info.node_type ==
			    IB_NODE_TYPE_CA) {
				status =
				    osmtest_get_path_rec_by_guid_pair(p_osmt,
								      p_src_node->
								      rec.
								      node_info.
								      port_guid,
								      p_dst_node->
								      rec.
								      node_info.
								      port_guid,
								      &context);

				/* In a case of TIMEOUT you still can try sending but cant count, maybe its a temporary issue */
				if (status != IB_SUCCESS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"ERR 0009: "
						"osmtest_get_path_rec_by_guid_pair failed (%s)\n",
						ib_get_err_str(status));
					if (status != IB_TIMEOUT)
						goto Exit;
				} else {
					/* we might have received several records */
					num_recs = context.result.result_cnt;
					/*
					 * Populate the database with the received records.
					 */
					*p_num_recs += num_recs;
					++*p_num_queries;
					OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
						"Received %u records\n", num_recs);
					/* Dont waste time if not VERBOSE and above */
					if (p_osmt->log.level & OSM_LOG_VERBOSE) {
						for (i = 0; i < num_recs; i++) {
							p_rec =
							    osmv_get_query_path_rec
							    (context.result.
							     p_result_madw, i);
							osm_dump_path_record
							    (&p_osmt->log,
							     p_rec,
							     OSM_LOG_VERBOSE);
						}
					}
				}
				if (context.result.p_result_madw != NULL) {
					osm_mad_pool_put(&p_osmt->mad_pool,
							 context.result.
							 p_result_madw);
					context.result.p_result_madw = NULL;
				}
			}
			/* next one please */
			p_dst_node =
			    (node_t *) cl_qmap_next(&p_dst_node->map_item);
		}

		p_src_node = (node_t *) cl_qmap_next(&p_src_node->map_item);
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}