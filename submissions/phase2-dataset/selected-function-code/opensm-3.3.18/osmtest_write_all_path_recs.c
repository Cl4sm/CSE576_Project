static ib_api_status_t
osmtest_write_all_path_recs(IN osmtest_t * const p_osmt, IN FILE * fh)
{
	osmtest_req_context_t context;
	const ib_path_rec_t *p_rec;
	cl_status_t status = CL_SUCCESS;
	int num_recs, i;
	cl_qmap_t *p_tbl;
	node_t *p_src_node, *p_dst_node;
	ib_api_status_t got_status = IB_SUCCESS;

	OSM_LOG_ENTER(&p_osmt->log);

	memset(&context, 0, sizeof(context));

	/*
	 * Go over all nodes that exist in the subnet
	 * for each pair that are not switch nodes get the path record
	 */

	context.p_osmt = p_osmt;

	p_tbl = &p_osmt->exp_subn.node_lid_tbl;

	p_src_node = (node_t *) cl_qmap_head(p_tbl);

	while (p_src_node != (node_t *) cl_qmap_end(p_tbl)) {
		/* HACK we use capability_mask to know diff a CA node from switch node */
		/* if(p_src_node->rec.node_info.capability_mask  ) { */
		p_dst_node = (node_t *) cl_qmap_head(p_tbl);

		while (p_dst_node != (node_t *) cl_qmap_end(p_tbl)) {
			/* HACK we use capability_mask to know diff a CA node from switch node */
			/* if (p_dst_node->rec.node_info.capability_mask) { */

			/* query for it: */
			status = osmtest_get_path_rec_by_lid_pair(p_osmt,
								  p_src_node->
								  rec.lid,
								  p_dst_node->
								  rec.lid,
								  &context);

			if (status != IB_SUCCESS) {
				OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 012D: "
					"failed to get path info from LID:0x%X To LID:0x%X (%s)\n",
					p_src_node->rec.lid,
					p_dst_node->rec.lid,
					ib_get_err_str(status));
				/* remember the first error status */
				got_status =
				    (got_status ==
				     IB_SUCCESS) ? status : got_status;
			} else {
				/* we might have received several records */
				num_recs = context.result.result_cnt;
				for (i = 0; i < num_recs; i++) {
					p_rec =
					    osmv_get_query_path_rec(context.
								    result.
								    p_result_madw,
								    i);
					osmtest_write_path_info(p_osmt, fh,
								p_rec);
				}
			}
/*  } */

			if (context.result.p_result_madw != NULL) {
				osm_mad_pool_put(&p_osmt->mad_pool,
						 context.result.p_result_madw);
				context.result.p_result_madw = NULL;
			}

			/* next one please */
			p_dst_node =
			    (node_t *) cl_qmap_next(&p_dst_node->map_item);
		}
/* } */

		p_src_node = (node_t *) cl_qmap_next(&p_src_node->map_item);
	}

	if (got_status != IB_SUCCESS)
		status = got_status;

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