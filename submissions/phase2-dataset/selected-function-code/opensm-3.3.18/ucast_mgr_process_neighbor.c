static void ucast_mgr_process_neighbor(IN osm_ucast_mgr_t * p_mgr,
				       IN osm_switch_t * p_this_sw,
				       IN osm_switch_t * p_remote_sw,
				       IN uint8_t port_num,
				       IN uint8_t remote_port_num)
{
	osm_switch_t *p_sw;
	cl_map_item_t *item;
	uint16_t lid_ho;
	uint8_t hops;
	osm_physp_t *p;

	OSM_LOG_ENTER(p_mgr->p_log);

	OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
		"Node 0x%" PRIx64 ", remote node 0x%" PRIx64
		", port %u, remote port %u\n",
		cl_ntoh64(osm_node_get_node_guid(p_this_sw->p_node)),
		cl_ntoh64(osm_node_get_node_guid(p_remote_sw->p_node)),
		port_num, remote_port_num);

	p = osm_node_get_physp_ptr(p_this_sw->p_node, port_num);

	for (item = cl_qmap_head(&p_mgr->p_subn->sw_guid_tbl);
	     item != cl_qmap_end(&p_mgr->p_subn->sw_guid_tbl);
	     item = cl_qmap_next(item)) {
		p_sw = (osm_switch_t *) item;
		lid_ho = cl_ntoh16(osm_node_get_base_lid(p_sw->p_node, 0));
		hops = osm_switch_get_least_hops(p_remote_sw, lid_ho);
		if (hops == OSM_NO_PATH)
			continue;
		hops += p->hop_wf;
		if (hops <
		    osm_switch_get_hop_count(p_this_sw, lid_ho, port_num)) {
			if (osm_switch_set_hops
			    (p_this_sw, lid_ho, port_num, hops) != 0)
				OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR, "ERR 3A03: "
					"cannot set hops for lid %u at switch 0x%"
					PRIx64 "\n", lid_ho,
					cl_ntoh64(osm_node_get_node_guid
						  (p_this_sw->p_node)));
			p_mgr->some_hop_count_set = TRUE;
		}
	}

	OSM_LOG_EXIT(p_mgr->p_log);
}