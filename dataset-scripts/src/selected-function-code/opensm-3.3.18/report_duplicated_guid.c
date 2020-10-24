static void report_duplicated_guid(IN osm_sm_t * sm, osm_physp_t * p_physp,
				   osm_node_t * p_neighbor_node,
				   const uint8_t port_num)
{
	osm_physp_t *p_old, *p_new;
	osm_dr_path_t path;

	p_old = p_physp->p_remote_physp;
	p_new = osm_node_get_physp_ptr(p_neighbor_node, port_num);

	OSM_LOG(sm->p_log, OSM_LOG_SYS | OSM_LOG_ERROR, "ERR 0D01: "
		"Found duplicated node GUID.\n"
		"Node 0x%" PRIx64 " port %u is reachable from remote node "
		"0x%" PRIx64 " port %u and remote node 0x%" PRIx64 " port %u.\n"
		"Paths are:\n",
		cl_ntoh64(p_physp->p_node->node_info.node_guid),
		p_physp->port_num,
		cl_ntoh64(p_old->p_node->node_info.node_guid), p_old->port_num,
		cl_ntoh64(p_new->p_node->node_info.node_guid), p_new->port_num);

	osm_dump_dr_path_v2(sm->p_log, osm_physp_get_dr_path_ptr(p_physp),
			    FILE_ID, OSM_LOG_ERROR);

	path = *osm_physp_get_dr_path_ptr(p_new);
	if (osm_dr_path_extend(&path, port_num))
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D05: "
			"DR path with hop count %d couldn't be extended\n",
			path.hop_count);
	osm_dump_dr_path_v2(sm->p_log, &path, FILE_ID, OSM_LOG_ERROR);
}