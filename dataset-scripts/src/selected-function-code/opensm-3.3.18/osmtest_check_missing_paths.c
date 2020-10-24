static ib_api_status_t osmtest_check_missing_paths(IN osmtest_t * const p_osmt)
{
	const path_t *p_path;
	cl_status_t status = IB_SUCCESS;
	cl_qmap_t *p_tbl;

	OSM_LOG_ENTER(&p_osmt->log);

	p_tbl = &p_osmt->exp_subn.path_tbl;

	p_path = (path_t *) cl_qmap_head(p_tbl);

	while (p_path != (path_t *) cl_qmap_end(p_tbl)) {
		if (p_path->count == 0) {
			/*
			 * This path was not reported by the SA
			 */
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0051: "
				"SA did not return path SLID 0x%X to DLID 0x%X\n",
				cl_ntoh16(p_path->rec.slid),
				cl_ntoh16(p_path->rec.dlid));
			status = IB_ERROR;
			goto Exit;
		}

		p_path = (path_t *) cl_qmap_next(&p_path->map_item);
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}