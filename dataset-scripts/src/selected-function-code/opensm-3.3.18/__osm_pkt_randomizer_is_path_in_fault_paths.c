boolean_t
__osm_pkt_randomizer_is_path_in_fault_paths(IN osm_log_t * p_log,
					    IN osm_dr_path_t * p_dr_path,
					    IN osm_pkt_randomizer_t *
					    p_pkt_rand)
{
	boolean_t res = FALSE, found_path;
	osm_dr_path_t *p_found_dr_path;
	uint8_t ind1, ind2;

	OSM_LOG_ENTER(p_log);

	for (ind1 = 0; ind1 < p_pkt_rand->num_paths_initialized; ind1++) {
		found_path = TRUE;
		p_found_dr_path = &(p_pkt_rand->fault_dr_paths[ind1]);
		/* if the hop count of the found path is greater than the
		   hop count of the input path - then it is not part of it.
		   Check the next path. */
		if (p_found_dr_path->hop_count > p_dr_path->hop_count)
			continue;

		/* go over all the ports in the found path and see if they match
		   the ports in the input path */
		for (ind2 = 0; ind2 <= p_found_dr_path->hop_count; ind2++)
			if (p_found_dr_path->path[ind2] !=
			    p_dr_path->path[ind2])
				found_path = FALSE;

		/* If found_path is TRUE  then there is a full match of the path */
		if (found_path == TRUE) {
			OSM_LOG(p_log, OSM_LOG_VERBOSE,
				"Given path is in a fault path\n");
			res = TRUE;
			break;
		}
	}

	OSM_LOG_EXIT(p_log);
	return res;
}