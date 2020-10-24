int osm_link_mgr_process(osm_sm_t * sm, IN const uint8_t link_state)
{
	cl_qmap_t *p_node_guid_tbl;
	osm_node_t *p_node;
	int ret = 0;

	OSM_LOG_ENTER(sm->p_log);

	p_node_guid_tbl = &sm->p_subn->node_guid_tbl;

	CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);

	for (p_node = (osm_node_t *) cl_qmap_head(p_node_guid_tbl);
	     p_node != (osm_node_t *) cl_qmap_end(p_node_guid_tbl);
	     p_node = (osm_node_t *) cl_qmap_next(&p_node->map_item))
		if (link_mgr_process_node(sm, p_node, link_state))
			ret = -1;

	CL_PLOCK_RELEASE(sm->p_lock);

	OSM_LOG_EXIT(sm->p_log);
	return ret;
}