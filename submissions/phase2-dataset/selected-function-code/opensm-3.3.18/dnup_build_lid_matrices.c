static int dnup_build_lid_matrices(IN dnup_t * p_dnup)
{
	int status;

	OSM_LOG_ENTER(&p_dnup->p_osm->log);

	OSM_LOG(&p_dnup->p_osm->log, OSM_LOG_VERBOSE,
		"Ranking all port guids in the list\n");
	/* Check if it's not a switched subnet */
	if (cl_is_qmap_empty(&p_dnup->p_osm->subn.sw_guid_tbl)) {
		OSM_LOG(&p_dnup->p_osm->log, OSM_LOG_ERROR, "ERR AEOB: "
			"This is not a switched subnet, cannot perform DNUP algorithm\n");
		status = -1;
		goto _exit;
	}

	/* Rank the subnet switches */
	dnup_subn_rank(p_dnup);

	/* After multiple ranking need to set Min Hop Table by DnUp algorithm  */
	OSM_LOG(&p_dnup->p_osm->log, OSM_LOG_VERBOSE,
		"Setting all switches' Min Hop Table\n");
	status = dnup_set_min_hop_table(p_dnup);

_exit:
	OSM_LOG_EXIT(&p_dnup->p_osm->log);
	return status;
}