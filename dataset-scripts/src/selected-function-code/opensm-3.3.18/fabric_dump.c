static void fabric_dump(ftree_fabric_t * p_ftree)
{
	uint32_t i;
	ftree_hca_t *p_hca;
	ftree_sw_t *p_sw;

	if (!OSM_LOG_IS_ACTIVE_V2(&p_ftree->p_osm->log, OSM_LOG_DEBUG))
		return;

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG, "\n"
		"                       |-------------------------------|\n"
		"                       |-  Full fabric topology dump  -|\n"
		"                       |-------------------------------|\n\n");

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG, "-- CAs:\n");

	for (p_hca = (ftree_hca_t *) cl_qmap_head(&p_ftree->hca_tbl);
	     p_hca != (ftree_hca_t *) cl_qmap_end(&p_ftree->hca_tbl);
	     p_hca = (ftree_hca_t *) cl_qmap_next(&p_hca->map_item)) {
		hca_dump(p_ftree, p_hca);
	}

	for (i = 0; i <= p_ftree->max_switch_rank; i++) {
		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
			"-- Rank %u switches\n", i);
		for (p_sw = (ftree_sw_t *) cl_qmap_head(&p_ftree->sw_tbl);
		     p_sw != (ftree_sw_t *) cl_qmap_end(&p_ftree->sw_tbl);
		     p_sw = (ftree_sw_t *) cl_qmap_next(&p_sw->map_item)) {
			if (p_sw->rank == i)
				sw_dump(p_ftree, p_sw);
		}
	}

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG, "\n"
		"                       |---------------------------------------|\n"
		"                       |- Full fabric topology dump completed -|\n"
		"                       |---------------------------------------|\n\n");
}				/* fabric_dump() */