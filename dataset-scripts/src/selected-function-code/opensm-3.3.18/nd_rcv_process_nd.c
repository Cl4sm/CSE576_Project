static void nd_rcv_process_nd(IN osm_sm_t * sm, IN osm_node_t * p_node,
			      IN const ib_node_desc_t * p_nd)
{
	char *tmp_desc;
	char print_desc[IB_NODE_DESCRIPTION_SIZE + 1];

	OSM_LOG_ENTER(sm->p_log);

	memcpy(&p_node->node_desc.description, p_nd, sizeof(*p_nd));

	/* also set up a printable version */
	memcpy(print_desc, p_nd, sizeof(*p_nd));
	print_desc[IB_NODE_DESCRIPTION_SIZE] = '\0';
	tmp_desc = remap_node_name(sm->p_subn->p_osm->node_name_map,
				   cl_ntoh64(osm_node_get_node_guid(p_node)),
				   print_desc);

	/* make a copy for this node to "own" */
	if (p_node->print_desc)
		free(p_node->print_desc);
	p_node->print_desc = tmp_desc;

#ifdef ENABLE_OSM_PERF_MGR
	/* update the perfmgr entry if available */
	osm_perfmgr_update_nodename(&sm->p_subn->p_osm->perfmgr,
				cl_ntoh64(osm_node_get_node_guid(p_node)),
				p_node->print_desc);
#endif				/* ENABLE_OSM_PERF_MGR */

	OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
		"Node 0x%" PRIx64 ", Description = %s\n",
		cl_ntoh64(osm_node_get_node_guid(p_node)), p_node->print_desc);

	OSM_LOG_EXIT(sm->p_log);
}