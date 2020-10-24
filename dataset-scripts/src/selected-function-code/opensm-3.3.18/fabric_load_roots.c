static boolean_t fabric_load_roots(IN ftree_fabric_t * p_ftree,
				   IN cl_list_t* p_ranking_bfs_list)
{
	struct rank_root_cxt context;
	unsigned num_roots;

	if (p_ranking_bfs_list) {

		/* Rank all the roots and add them to list */
		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
			"Fetching root nodes from file %s\n",
			p_ftree->p_osm->subn.opt.root_guid_file);

		context.fabric = p_ftree;
		context.list = p_ranking_bfs_list;
		if (parse_node_map(p_ftree->p_osm->subn.opt.root_guid_file,
				   rank_root_sw_by_guid, &context)) {
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR, "ERR AB2A: "
				"cannot parse root guids file \'%s\'\n",
				p_ftree->p_osm->subn.opt.root_guid_file);
			return FALSE;
		}

		num_roots = cl_list_count(p_ranking_bfs_list);
		if (!num_roots) {
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR, "ERR AB25: "
				"No valid roots supplied\n");
			return FALSE;
		}

		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
			"Ranked %u valid root switches\n", num_roots);
	}
	return TRUE;
}