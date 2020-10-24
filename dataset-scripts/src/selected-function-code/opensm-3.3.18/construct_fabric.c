static int construct_fabric(IN void *context)
{
	ftree_fabric_t *p_ftree = context;
	int status = 0;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	fabric_clear(p_ftree);

	if (p_ftree->p_osm->subn.opt.lmc > 0) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_INFO, FILE_ID,
			   "LMC > 0 is not supported by fat-tree routing.\n"
			   "Falling back to default routing\n");
		status = -1;
		goto Exit;
	}

	if (cl_qmap_count(&p_ftree->p_osm->subn.sw_guid_tbl) < 2) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_INFO, FILE_ID,
			   "Fabric has %u switches - topology is not fat-tree.\n"
			   "Falling back to default routing\n",
			   cl_qmap_count(&p_ftree->p_osm->subn.sw_guid_tbl));
		status = -1;
		goto Exit;
	}

	if ((cl_qmap_count(&p_ftree->p_osm->subn.node_guid_tbl) -
	     cl_qmap_count(&p_ftree->p_osm->subn.sw_guid_tbl)) < 2) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_INFO, FILE_ID,
			   "Fabric has %u nodes (%u switches) - topology is not fat-tree.\n"
			   "Falling back to default routing\n",
			   cl_qmap_count(&p_ftree->p_osm->subn.node_guid_tbl),
			   cl_qmap_count(&p_ftree->p_osm->subn.sw_guid_tbl));
		status = -1;
		goto Exit;
	}

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE, "\n"
		"                       |----------------------------------------|\n"
		"                       |- Starting FatTree fabric construction -|\n"
		"                       |----------------------------------------|\n\n");

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
		"Populating FatTree Switch and CA tables\n");
	if (fabric_populate_nodes(p_ftree) != 0) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_INFO, FILE_ID,
			   "Fabric topology is not fat-tree - "
			   "falling back to default routing\n");
		status = -1;
		goto Exit;
	}

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
		"Reading guid files provided by user\n");
	if (fabric_read_guid_files(p_ftree) != 0) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_INFO, FILE_ID,
			   "Failed reading guid files - "
			   "falling back to default routing\n");
		status = -1;
		goto Exit;
	}

	if (cl_qmap_count(&p_ftree->hca_tbl) < 2) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_INFO, FILE_ID,
			   "Fabric has %u CAs - topology is not fat-tree.\n"
			   "Falling back to default routing\n",
			   cl_qmap_count(&p_ftree->hca_tbl));
		status = -1;
		goto Exit;
	}

	/* Rank all the switches in the fabric.
	   After that we will know only fabric max switch rank.
	   We will be able to check leaf switches rank and the
	   whole tree rank after filling ports and marking CNs. */
	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE, "Ranking FatTree\n");
	if (fabric_rank(p_ftree) != 0) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_INFO, FILE_ID,
			   "Failed ranking the tree\n");
		status = -1;
		goto Exit;
	}
	fabric_remove_unranked_sw(p_ftree);

	/* For each hca and switch, construct array of ports.
	   This is done after the whole FatTree data structure is ready,
	   because we want the ports to have pointers to ftree_{sw,hca}_t
	   objects, and we need the switches to be already ranked because
	   that's how the port direction is determined. */
	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
		"Populating CA & switch ports\n");
	if (fabric_populate_ports(p_ftree) != 0) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_INFO, FILE_ID,
			   "Fabric topology is not a fat-tree\n");
		status = -1;
		goto Exit;
	} else if (p_ftree->cn_num == 0) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_INFO, FILE_ID,
			   "Fabric has no valid compute nodes\n");
		status = -1;
		goto Exit;
	}

	/* Now that the CA ports have been created and CNs were marked,
	   we can complete the fabric ranking - set leaf switches rank. */
	fabric_set_leaf_rank(p_ftree);

	if (fabric_get_rank(p_ftree) > FAT_TREE_MAX_RANK ||
	    fabric_get_rank(p_ftree) < FAT_TREE_MIN_RANK) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_INFO, FILE_ID,
			   "Fabric rank is %u (should be between %u and %u)\n",
			   fabric_get_rank(p_ftree), FAT_TREE_MIN_RANK,
			   FAT_TREE_MAX_RANK);
		status = -1;
		goto Exit;
	}

	/* Mark all the switches in the fabric with rank equal to
	   p_ftree->leaf_switch_rank and that are also connected to CNs.
	   As a by-product, this function also runs basic topology
	   validation - it checks that all the CNs are at the same rank. */
	if (fabric_mark_leaf_switches(p_ftree)) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_INFO, FILE_ID,
			   "Fabric topology is not a fat-tree\n");
		status = -1;
		goto Exit;
	}

	/* Assign index to all the switches in the fabric.
	   This function also sorts leaf switch array by the switch index,
	   sorts all the port arrays of the indexed switches by remote
	   switch index, and creates switch-by-tuple table (sw_by_tuple_tbl) */
	fabric_make_indexing(p_ftree);

	/* Create leaf switch array sorted by index.
	   This array contains switches with rank equal to p_ftree->leaf_switch_rank
	   and that are also connected to CNs (REAL leafs), and it may contain
	   switches at the same leaf rank w/o CNs, if this is the order of indexing.
	   In any case, the first and the last switches in the array are REAL leafs. */
	if (fabric_create_leaf_switch_array(p_ftree)) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_INFO, FILE_ID,
			   "Fabric topology is not a fat-tree\n");
		status = -1;
		goto Exit;
	}

	/* calculate and set ftree.max_cn_per_leaf field */
	fabric_set_max_cn_per_leaf(p_ftree);

	/* print general info about fabric topology */
	fabric_dump_general_info(p_ftree);

	/* dump full tree topology */
	if (OSM_LOG_IS_ACTIVE_V2(&p_ftree->p_osm->log, OSM_LOG_DEBUG))
		fabric_dump(p_ftree);

	/* the fabric is required to be PURE fat-tree only if the root
	   guid file hasn't been provided by user */
	if (!fabric_roots_provided(p_ftree) &&
	    !fabric_validate_topology(p_ftree)) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_INFO, FILE_ID,
			   "Fabric topology is not a fat-tree\n");
		status = -1;
		goto Exit;
	}

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
		"Max LID in switch LFTs: %u\n", p_ftree->lft_max_lid);

	/* Build the full lid matrices needed for multicast routing */
	osm_ucast_mgr_build_lid_matrices(&p_ftree->p_osm->sm.ucast_mgr);

Exit:
	if (status != 0) {
		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
			"Clearing FatTree Fabric data structures\n");
		fabric_clear(p_ftree);
	} else
		p_ftree->fabric_built = TRUE;

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE, "\n"
		"                       |--------------------------------------------------|\n"
		"                       |- Done constructing FatTree fabric (status = %d) -|\n"
		"                       |--------------------------------------------------|\n\n",
		status);

	OSM_LOG_EXIT(&p_ftree->p_osm->log);
	return status;
}				/* construct_fabric() */