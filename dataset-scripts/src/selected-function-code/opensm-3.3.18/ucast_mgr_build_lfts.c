static int ucast_mgr_build_lfts(osm_ucast_mgr_t * p_mgr)
{
	cl_qlist_init(&p_mgr->port_order_list);

	if (p_mgr->p_subn->opt.guid_routing_order_file) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Fetching guid routing order file \'%s\'\n",
			p_mgr->p_subn->opt.guid_routing_order_file);

		if (parse_node_map(p_mgr->p_subn->opt.guid_routing_order_file,
				   add_guid_to_order_list, p_mgr))
			OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR, "ERR 3A0D: "
				"cannot parse guid routing order file \'%s\'\n",
				p_mgr->p_subn->opt.guid_routing_order_file);
	}
	sort_ports_by_switch_load(p_mgr);

	if (p_mgr->p_subn->opt.port_prof_ignore_file) {
		cl_qmap_apply_func(&p_mgr->p_subn->sw_guid_tbl,
				   clear_prof_ignore_flag, NULL);
		if (parse_node_map(p_mgr->p_subn->opt.port_prof_ignore_file,
				   mark_ignored_port, p_mgr)) {
			OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR, "ERR 3A0E: "
				"cannot parse port prof ignore file \'%s\'\n",
				p_mgr->p_subn->opt.port_prof_ignore_file);
		}
	}

	cl_qmap_apply_func(&p_mgr->p_subn->port_guid_tbl,
			   add_port_to_order_list, p_mgr);

	cl_qmap_apply_func(&p_mgr->p_subn->sw_guid_tbl, ucast_mgr_process_tbl,
			   p_mgr);

	cl_qlist_remove_all(&p_mgr->port_order_list);

	return 0;
}