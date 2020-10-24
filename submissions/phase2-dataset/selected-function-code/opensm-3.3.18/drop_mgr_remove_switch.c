static void drop_mgr_remove_switch(osm_sm_t * sm, IN osm_node_t * p_node)
{
	osm_switch_t *p_sw;
	cl_qmap_t *p_sw_guid_tbl;
	ib_net64_t node_guid;

	OSM_LOG_ENTER(sm->p_log);

	node_guid = osm_node_get_node_guid(p_node);
	p_sw_guid_tbl = &sm->p_subn->sw_guid_tbl;

	p_sw = (osm_switch_t *) cl_qmap_remove(p_sw_guid_tbl, node_guid);
	if (p_sw == (osm_switch_t *) cl_qmap_end(p_sw_guid_tbl)) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0102: "
			"Node 0x%016" PRIx64 " not in switch table\n",
			cl_ntoh64(osm_node_get_node_guid(p_node)));
	} else {
		p_node->sw = NULL;
		osm_switch_delete(&p_sw);
	}

	OSM_LOG_EXIT(sm->p_log);
}