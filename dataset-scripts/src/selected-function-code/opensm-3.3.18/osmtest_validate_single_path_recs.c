static ib_api_status_t
osmtest_validate_single_path_recs(IN osmtest_t * const p_osmt)
{
	path_t *p_path;
	cl_status_t status = IB_SUCCESS;
	const cl_qmap_t *p_path_tbl;
/* We skip node to node path record validation since it might contains
   NONEXISTENT PATHS, i.e. when using UPDN */
	osmv_guid_pair_t guid_pair;
	uint16_t cnt;

	OSM_LOG_ENTER(&p_osmt->log);

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
		"Validating individual path record queries\n");
	p_path_tbl = &p_osmt->exp_subn.path_tbl;

	osmtest_prepare_db(p_osmt);

	/*
	 * Walk the list of all path records, and ask for each one
	 * specifically.  Make sure we get it.
	 */
	cnt = 0;
	p_path = (path_t *) cl_qmap_head(p_path_tbl);
	while (p_path != (path_t *) cl_qmap_end(p_path_tbl)) {
		status =
		    osmtest_validate_single_path_rec_lid_pair(p_osmt, p_path);
		if (status != IB_SUCCESS)
			goto Exit;
		cnt++;
		p_path = (path_t *) cl_qmap_next(&p_path->map_item);
	}

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
		"Total of %u path records validated using LID based query\n",
		cnt);

	status = osmtest_check_missing_paths(p_osmt);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0115: "
			"osmtest_check_missing_paths failed (%s)\n",
			ib_get_err_str(status));
		goto Exit;
	}

	/*
	 * Do the whole thing again with port GUID pairs.
	 * Note that multiple path records may be returned
	 * for each guid pair if LMC > 0.
	 */
	osmtest_prepare_db(p_osmt);
	cnt = 0;
	p_path = (path_t *) cl_qmap_head(p_path_tbl);
	while (p_path != (path_t *) cl_qmap_end(p_path_tbl)) {
		guid_pair.src_guid = p_path->rec.sgid.unicast.interface_id;
		guid_pair.dest_guid = p_path->rec.dgid.unicast.interface_id;
		status = osmtest_validate_single_path_rec_guid_pair(p_osmt,
								    &guid_pair);
		if (status != IB_SUCCESS)
			goto Exit;
		cnt++;
		p_path = (path_t *) cl_qmap_next(&p_path->map_item);
	}

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
		"Total of %u path records validated using GUID based query\n",
		cnt);

	status = osmtest_check_missing_paths(p_osmt);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0116: "
			"osmtest_check_missing_paths failed (%s)\n",
			ib_get_err_str(status));
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}