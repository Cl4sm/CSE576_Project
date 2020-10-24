static ib_api_status_t
osmtest_validate_port_rec(IN osmtest_t * const p_osmt,
			  IN const ib_portinfo_record_t * const p_rec)
{
	cl_status_t status = IB_SUCCESS;
	port_t *p_port;
	const cl_qmap_t *p_tbl;

	OSM_LOG_ENTER(&p_osmt->log);

	/*
	 * Find proper port record in the database.
	 * (we use by guid - since lid is not unique)
	 */
	p_tbl = &p_osmt->exp_subn.port_key_tbl;
	p_port =
	    (port_t *) cl_qmap_get(p_tbl,
				   port_gen_id(p_rec->lid, p_rec->port_num));
	if (p_port == (port_t *) cl_qmap_end(p_tbl)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0094: "
			"Unexpected port LID 0x%X, Num:0x%X\n",
			cl_ntoh16(p_rec->lid), p_rec->port_num);
		status = IB_ERROR;
		goto Exit;
	}

	status = osmtest_validate_port_data(p_osmt, p_port, p_rec);

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}