static ib_api_status_t
osmtest_validate_path_rec(IN osmtest_t * const p_osmt,
			  IN const ib_path_rec_t * const p_rec)
{
	cl_status_t status = IB_SUCCESS;
	path_t *p_path;
	const cl_qmap_t *p_tbl;

	OSM_LOG_ENTER(&p_osmt->log);

	/*
	 * Find proper path record in the database.
	 */
	p_tbl = &p_osmt->exp_subn.path_tbl;
	p_path = (path_t *) cl_qmap_get(p_tbl, osmtest_path_rec_key_get(p_rec));
	if (p_path == (path_t *) cl_qmap_end(p_tbl)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0095: "
			"Unexpected path SLID 0x%X to DLID 0x%X\n",
			cl_ntoh16(p_rec->slid), cl_ntoh16(p_rec->dlid));
		status = IB_ERROR;
		goto Exit;
	}

	status = osmtest_validate_path_data(p_osmt, p_path, p_rec);

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}