static ib_api_status_t
osmtest_validate_node_rec(IN osmtest_t * const p_osmt,
			  IN const ib_node_record_t * const p_rec)
{
	cl_status_t status = IB_SUCCESS;
	node_t *p_node;
	const cl_qmap_t *p_tbl;

	OSM_LOG_ENTER(&p_osmt->log);

	/*
	 * Find proper node record in the database.
	 */
	p_tbl = &p_osmt->exp_subn.node_lid_tbl;
	p_node = (node_t *) cl_qmap_get(p_tbl, p_rec->lid);
	if (p_node == (node_t *) cl_qmap_end(p_tbl)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0035: "
			"Unexpected node 0x%016" PRIx64 ", LID 0x%X\n",
			cl_ntoh64(p_rec->node_info.node_guid),
			cl_ntoh16(p_rec->lid));
		status = IB_ERROR;
		goto Exit;
	}

	status = osmtest_validate_node_data(p_osmt, p_node, p_rec);

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}