static ib_api_status_t
osmtest_write_all_port_recs(IN osmtest_t * const p_osmt, IN FILE * fh)
{
	osmtest_req_context_t context;
	const ib_node_record_t *p_node_rec;
	const ib_portinfo_record_t *p_rec;
	uint8_t port_num;
	cl_status_t status = CL_SUCCESS;
	cl_qmap_t *p_tbl;
	node_t *p_node;
	port_t *p_port;
	int result;

	OSM_LOG_ENTER(&p_osmt->log);

	memset(&context, 0, sizeof(context));

	/* print header */
	result = fprintf(fh, "#\n" "# PortInfo Records\n" "#\n");
	if (result < 0) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0029: "
			"Write failed\n");
		status = IB_ERROR;
		goto Exit;
	}

	/* use the pre-explored set of nodes */
	p_tbl = &p_osmt->exp_subn.node_lid_tbl;
	p_node = (node_t *) cl_qmap_head(p_tbl);

	/*
	 * Go over all LIDs in the range 1 to max_lid and do a
	 * NodeRecord query by that lid.
	 */
	while (p_node != (node_t *) cl_qmap_end(p_tbl)) {

		p_node_rec = &(p_node->rec);

		/* go through all ports of the node: */
		for (port_num = 0; port_num <= p_node_rec->node_info.num_ports;
		     port_num++) {
			/* prepare the query context */
			memset(&context, 0, sizeof(context));

			status = osmtest_get_port_rec_by_num(p_osmt,
							     p_node_rec->lid,
							     port_num,
							     &context);
			if (status != IB_SUCCESS) {
				if (status != IB_SA_MAD_STATUS_NO_RECORDS) {
					OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
						"WRN 0122: "
						"Error encountered getting port info for LID:0x%04X Num:0x%02X (%s)\n",
						p_node_rec->lid, port_num,
						ib_get_err_str(status));
					goto Exit;
				} else {
					OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
						"WRN 0123: "
						"failed to get port info for LID:0x%04X Num:0x%02X (%s)\n",
						p_node_rec->lid, port_num,
						ib_get_err_str(status));
					status = IB_SUCCESS;
				}
			} else {
				/* OK we got something */
				p_rec =
				    osmv_get_query_portinfo_rec(context.result.
								p_result_madw,
								0);
				osmtest_write_port_info(p_osmt, fh, p_rec);

				/* create a subnet object */
				p_port = port_new();
				CL_ASSERT(p_port != NULL);

				/* copy the info to the subnet node object */
				p_port->rec = *p_rec;

				cl_qmap_insert(&p_osmt->exp_subn.port_key_tbl,
					       port_gen_id(p_node_rec->lid,
							   port_num),
					       &p_port->map_item);
			}

			if (context.result.p_result_madw != NULL) {
				osm_mad_pool_put(&p_osmt->mad_pool,
						 context.result.p_result_madw);
				context.result.p_result_madw = NULL;
			}
		}
		p_node = (node_t *) cl_qmap_next(&p_node->map_item);
	}

	/* we must set the exist status to avoid abort of the over all algorith */

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