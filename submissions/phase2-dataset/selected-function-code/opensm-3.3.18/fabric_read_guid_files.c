static int fabric_read_guid_files(IN ftree_fabric_t * p_ftree)
{
	int status = 0;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	if (fabric_cns_provided(p_ftree)) {
		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
			"Fetching compute nodes from file %s\n",
			p_ftree->p_osm->subn.opt.cn_guid_file);

		if (parse_node_map(p_ftree->p_osm->subn.opt.cn_guid_file,
				   add_guid_item_to_map,
				   &p_ftree->cn_guid_tbl)) {
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR,
				"ERR AB23: " "Problem parsing CN guid file\n");
			status = -1;
			goto Exit;
		}

		if (!cl_qmap_count(&p_ftree->cn_guid_tbl)) {
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR,
				"ERR AB27: "
				"Compute node guids file has no valid guids\n");
			status = -1;
			goto Exit;
		}
	}

	if (fabric_ios_provided(p_ftree)) {
		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
			"Fetching I/O nodes from file %s\n",
			p_ftree->p_osm->subn.opt.io_guid_file);

		if (parse_node_map(p_ftree->p_osm->subn.opt.io_guid_file,
				   add_guid_item_to_map,
				   &p_ftree->io_guid_tbl)) {
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR,
				"ERR AB28: Problem parsing I/O guid file\n");
			status = -1;
			goto Exit;
		}

		if (!cl_qmap_count(&p_ftree->io_guid_tbl)) {
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR,
				"ERR AB29: "
				"I/O node guids file has no valid guids\n");
			status = -1;
			goto Exit;
		}
	}
Exit:
	OSM_LOG_EXIT(&p_ftree->p_osm->log);
	return status;
}				/*fabric_read_guid_files() */