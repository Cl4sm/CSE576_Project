static int ucast_mgr_setup_all_switches(osm_subn_t * p_subn)
{
	osm_switch_t *p_sw;
	uint16_t lids;

	lids = (uint16_t) cl_ptr_vector_get_size(&p_subn->port_lid_tbl);
	lids = lids ? lids - 1 : 0;

	for (p_sw = (osm_switch_t *) cl_qmap_head(&p_subn->sw_guid_tbl);
	     p_sw != (osm_switch_t *) cl_qmap_end(&p_subn->sw_guid_tbl);
	     p_sw = (osm_switch_t *) cl_qmap_next(&p_sw->map_item)) {
		if (osm_switch_prepare_path_rebuild(p_sw, lids)) {
			OSM_LOG(&p_subn->p_osm->log, OSM_LOG_ERROR, "ERR 3A0B: "
				"cannot setup switch 0x%016" PRIx64 "\n",
				cl_ntoh64(osm_node_get_node_guid
					  (p_sw->p_node)));
			return -1;
		}
		if (p_sw->search_ordering_ports) {
			free(p_sw->search_ordering_ports);
			p_sw->search_ordering_ports = NULL;
		}
	}

	if (p_subn->opt.port_search_ordering_file) {
		OSM_LOG(&p_subn->p_osm->log, OSM_LOG_DEBUG,
			"Fetching dimension ports file \'%s\'\n",
			p_subn->opt.port_search_ordering_file);
		if (parse_node_map(p_subn->opt.port_search_ordering_file,
				   set_search_ordering_ports, p_subn)) {
			OSM_LOG(&p_subn->p_osm->log, OSM_LOG_ERROR, "ERR 3A0F: "
				"cannot parse port_search_ordering_file \'%s\'\n",
				p_subn->opt.port_search_ordering_file);
		}
	}

	return 0;
}