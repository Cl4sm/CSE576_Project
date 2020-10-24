static void ucast_mgr_process_tbl(IN cl_map_item_t * p_map_item,
				  IN void *context)
{
	osm_ucast_mgr_t *p_mgr = context;
	osm_switch_t * p_sw = (osm_switch_t *) p_map_item;
	unsigned i, lids_per_port;

	OSM_LOG_ENTER(p_mgr->p_log);

	CL_ASSERT(p_sw && p_sw->p_node);

	OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
		"Processing switch 0x%" PRIx64 "\n",
		cl_ntoh64(osm_node_get_node_guid(p_sw->p_node)));

	/* Initialize LIDs in buffer to invalid port number. */
	memset(p_sw->new_lft, OSM_NO_PATH, p_sw->max_lid_ho + 1);

	alloc_ports_priv(p_mgr);

	/*
	   Iterate through every port setting LID routes for each
	   port based on base LID and LMC value.
	 */
	lids_per_port = 1 << p_mgr->p_subn->opt.lmc;
	for (i = 0; i < lids_per_port; i++) {
		cl_qlist_t *list = &p_mgr->port_order_list;
		cl_list_item_t *item;
		for (item = cl_qlist_head(list); item != cl_qlist_end(list);
		     item = cl_qlist_next(item)) {
			osm_port_t *port = cl_item_obj(item, port, list_item);
			ucast_mgr_process_port(p_mgr, p_sw, port, i);
		}
	}

	free_ports_priv(p_mgr);

	OSM_LOG_EXIT(p_mgr->p_log);
}