static int updn_lid_matrices(void *ctx)
{
	updn_t *p_updn = ctx;
	cl_map_item_t *item;
	osm_switch_t *p_sw;
	int ret = 0;

	OSM_LOG_ENTER(&p_updn->p_osm->log);

	for (item = cl_qmap_head(&p_updn->p_osm->subn.sw_guid_tbl);
	     item != cl_qmap_end(&p_updn->p_osm->subn.sw_guid_tbl);
	     item = cl_qmap_next(item)) {
		p_sw = (osm_switch_t *)item;
		p_sw->priv = create_updn_node(p_sw);
		if (!p_sw->priv) {
			OSM_LOG(&(p_updn->p_osm->log), OSM_LOG_ERROR, "ERR AA0C: "
				"cannot create updn node\n");
			OSM_LOG_EXIT(&p_updn->p_osm->log);
			return -1;
		}
	}

	/* First setup root nodes */
	p_updn->num_roots = 0;

	if (p_updn->p_osm->subn.opt.root_guid_file) {
		OSM_LOG(&p_updn->p_osm->log, OSM_LOG_DEBUG,
			"UPDN - Fetching root nodes from file \'%s\'\n",
			p_updn->p_osm->subn.opt.root_guid_file);

		ret = parse_node_map(p_updn->p_osm->subn.opt.root_guid_file,
				     rank_root_node, p_updn);
		if (ret) {
			OSM_LOG(&p_updn->p_osm->log, OSM_LOG_ERROR, "ERR AA02: "
				"cannot parse root guids file \'%s\'\n",
				p_updn->p_osm->subn.opt.root_guid_file);
			osm_ucast_mgr_build_lid_matrices(&p_updn->p_osm->sm.ucast_mgr);
			updn_find_root_nodes_by_min_hop(p_updn);
		} else if (p_updn->p_osm->subn.opt.connect_roots &&
			   p_updn->num_roots > 1)
			osm_ucast_mgr_build_lid_matrices(&p_updn->p_osm->sm.ucast_mgr);
	} else {
		osm_ucast_mgr_build_lid_matrices(&p_updn->p_osm->sm.ucast_mgr);
		updn_find_root_nodes_by_min_hop(p_updn);
	}

	if (p_updn->p_osm->subn.opt.ids_guid_file) {
		OSM_LOG(&p_updn->p_osm->log, OSM_LOG_DEBUG,
			"UPDN - update node ids from file \'%s\'\n",
			p_updn->p_osm->subn.opt.ids_guid_file);

		ret = parse_node_map(p_updn->p_osm->subn.opt.ids_guid_file,
				     update_id, p_updn->p_osm);
		if (ret)
			OSM_LOG(&p_updn->p_osm->log, OSM_LOG_ERROR, "ERR AA03: "
				"cannot parse node ids file \'%s\'\n",
				p_updn->p_osm->subn.opt.ids_guid_file);
	}

	/* Only if there are assigned root nodes do the algorithm, otherwise perform do nothing */
	if (p_updn->num_roots) {
		OSM_LOG(&p_updn->p_osm->log, OSM_LOG_DEBUG,
			"activating UPDN algorithm\n");
		ret = updn_build_lid_matrices(p_updn);
	} else {
		OSM_LOG(&p_updn->p_osm->log, OSM_LOG_INFO,
			"disabling UPDN algorithm, no root nodes were found\n");
		ret = -1;
	}

	if (OSM_LOG_IS_ACTIVE_V2(&p_updn->p_osm->log, OSM_LOG_ROUTING))
		osm_dump_qmap_to_file(p_updn->p_osm, "opensm-updn-roots.dump",
				      &p_updn->p_osm->subn.sw_guid_tbl,
				      dump_roots, NULL);

	for (item = cl_qmap_head(&p_updn->p_osm->subn.sw_guid_tbl);
	     item != cl_qmap_end(&p_updn->p_osm->subn.sw_guid_tbl);
	     item = cl_qmap_next(item)) {
		p_sw = (osm_switch_t *) item;
		delete_updn_node(p_sw->priv);
	}

	OSM_LOG_EXIT(&p_updn->p_osm->log);
	return ret;
}