static ib_api_status_t osmtest_check_missing_nodes(IN osmtest_t * const p_osmt)
{
	const node_t *p_node;
	cl_status_t status = IB_SUCCESS;
	cl_qmap_t *p_tbl;

	OSM_LOG_ENTER(&p_osmt->log);

	p_tbl = &p_osmt->exp_subn.node_lid_tbl;

	p_node = (node_t *) cl_qmap_head(p_tbl);

	while (p_node != (node_t *) cl_qmap_end(p_tbl)) {
		if (p_node->count == 0) {
			/*
			 * This node was not reported by the SA
			 */
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0080: "
				"Missing node 0x%016" PRIx64 "\n",
				cl_ntoh64(p_node->rec.node_info.node_guid));
			status = IB_ERROR;
		}

		p_node = (node_t *) cl_qmap_next(&p_node->map_item);
	}

	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}