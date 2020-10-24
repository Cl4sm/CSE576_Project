static void state_mgr_get_node_desc(IN cl_map_item_t * obj, IN void *context)
{
	osm_node_t *p_node = (osm_node_t *) obj;
	osm_sm_t *sm = context;

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_node);

	if (p_node->print_desc
	    && strcmp(p_node->print_desc, OSM_NODE_DESC_UNKNOWN))
		/* if ND is valid, do nothing */
		goto exit;

	OSM_LOG(sm->p_log, OSM_LOG_ERROR,
		"ERR 3319: Unknown node description for node GUID "
		"0x%016" PRIx64 ".  Reissuing ND query\n",
		cl_ntoh64(osm_node_get_node_guid(p_node)));

	state_mgr_update_node_desc(obj, context);

exit:
	OSM_LOG_EXIT(sm->p_log);
}