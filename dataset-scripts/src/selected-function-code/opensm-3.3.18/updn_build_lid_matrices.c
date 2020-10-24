static int updn_build_lid_matrices(IN updn_t * p_updn)
{
	int status;

	OSM_LOG_ENTER(&p_updn->p_osm->log);

	OSM_LOG(&p_updn->p_osm->log, OSM_LOG_VERBOSE,
		"Ranking all port guids in the list\n");
	if (!p_updn->num_roots) {
		OSM_LOG(&p_updn->p_osm->log, OSM_LOG_ERROR, "ERR AA0A: "
			"No guids were provided or number of guids is 0\n");
		status = -1;
		goto _exit;
	}

	/* Check if it's not a switched subnet */
	if (cl_is_qmap_empty(&p_updn->p_osm->subn.sw_guid_tbl)) {
		OSM_LOG(&p_updn->p_osm->log, OSM_LOG_ERROR, "ERR AA0B: "
			"This is not a switched subnet, cannot perform UPDN algorithm\n");
		status = -1;
		goto _exit;
	}

	/* Rank the subnet switches */
	updn_subn_rank(p_updn);

	/* After multiple ranking need to set Min Hop Table by UpDn algorithm  */
	OSM_LOG(&p_updn->p_osm->log, OSM_LOG_VERBOSE,
		"Setting all switches' Min Hop Table\n");
	status = updn_set_min_hop_table(p_updn);

_exit:
	OSM_LOG_EXIT(&p_updn->p_osm->log);
	return status;
}