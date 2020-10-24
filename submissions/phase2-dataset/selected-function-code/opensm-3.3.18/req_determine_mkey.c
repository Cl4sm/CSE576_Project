static ib_net64_t req_determine_mkey(IN osm_sm_t * sm,
				     IN const osm_dr_path_t * p_path)
{
	osm_node_t *p_node;
	osm_port_t *p_sm_port;
	osm_physp_t *p_physp;
	ib_net64_t dest_port_guid, m_key;
	uint8_t hop;

	OSM_LOG_ENTER(sm->p_log);

	p_physp = NULL;

	p_sm_port = osm_get_port_by_guid(sm->p_subn, sm->p_subn->sm_port_guid);

	/* hop_count == 0: destination port guid is SM */
	if (p_path->hop_count == 0) {
		dest_port_guid = sm->p_subn->sm_port_guid;
		goto Remote_Guid;
	}

	if (p_sm_port) {
		p_node = p_sm_port->p_node;
		if (osm_node_get_type(p_node) == IB_NODE_TYPE_SWITCH)
			p_physp = osm_node_get_physp_ptr(p_node, p_path->path[1]);
		else
			p_physp = p_sm_port->p_physp;
	}

	/* hop_count == 1: outgoing physp is SM physp */
	for (hop = 2; p_physp && hop <= p_path->hop_count; hop++) {
		p_physp = p_physp->p_remote_physp;
		if (!p_physp)
			break;
		p_node = p_physp->p_node;
		p_physp = osm_node_get_physp_ptr(p_node, p_path->path[hop]);
	}

	/* At this point, p_physp points at the outgoing physp on the
	   last hop, or NULL if we don't know it.
	*/
	if (!p_physp) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR,
			"ERR 1107: Outgoing physp is null on non-hop_0!\n");
		osm_dump_dr_path_v2(sm->p_log, p_path, FILE_ID, OSM_LOG_ERROR);
		dest_port_guid = 0;
		goto Remote_Guid;
	}

	if (p_physp->p_remote_physp) {
		dest_port_guid = p_physp->p_remote_physp->port_guid;
		goto Remote_Guid;
	}

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG, "Target port guid unknown, "
		"using persistent DB\n");
	if (!osm_db_neighbor_get(sm->p_subn->p_neighbor,
				 cl_ntoh64(p_physp->port_guid),
				 p_physp->port_num,
				 &dest_port_guid, NULL)) {
		dest_port_guid = cl_hton64(dest_port_guid);
	}

Remote_Guid:
	if (dest_port_guid) {
		if (!osm_db_guid2mkey_get(sm->p_subn->p_g2m,
					  cl_ntoh64(dest_port_guid), &m_key)) {
			m_key = cl_hton64(m_key);
			OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
				"Found mkey for guid 0x%"
				PRIx64 "\n", cl_ntoh64(dest_port_guid));
		} else {
			OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
				"Target port mkey unknown, using default\n");
			m_key = sm->p_subn->opt.m_key;
		}
	} else {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Target port guid unknown, using default\n");
		m_key = sm->p_subn->opt.m_key;
	}

	OSM_LOG_EXIT(sm->p_log);

	return m_key;
}