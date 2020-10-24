static void ni_rcv_process_existing(IN osm_sm_t * sm, IN osm_node_t * p_node,
				    IN const osm_madw_t * p_madw)
{
	ib_node_info_t *p_ni;
	ib_smp_t *p_smp;
	osm_ni_context_t *p_ni_context;
	uint8_t port_num;

	OSM_LOG_ENTER(sm->p_log);

	p_smp = osm_madw_get_smp_ptr(p_madw);
	p_ni = ib_smp_get_payload_ptr(p_smp);
	p_ni_context = osm_madw_get_ni_context_ptr(p_madw);
	port_num = ib_node_info_get_local_port_num(p_ni);

	OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
		"Rediscovered %s node 0x%" PRIx64 " TID 0x%" PRIx64
		", discovered %u times already\n",
		ib_get_node_type_str(p_ni->node_type),
		cl_ntoh64(p_ni->node_guid),
		cl_ntoh64(p_smp->trans_id), p_node->discovery_count);

	if (PF(port_num > p_ni->num_ports)) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D0C: "
			"Existing %s node GUID 0x%" PRIx64 "is non-compliant "
			"and is being ignored since the "
			"local port num %u > num ports %u\n",
			ib_get_node_type_str(p_ni->node_type),
			cl_ntoh64(p_ni->node_guid), port_num,
			p_ni->num_ports);
		goto Exit;
	}

	/*
	   If we haven't already encountered this existing node
	   on this particular sweep, then process further.
	 */
	p_node->discovery_count++;

	switch (p_ni->node_type) {
	case IB_NODE_TYPE_CA:
	case IB_NODE_TYPE_ROUTER:
		ni_rcv_process_existing_ca_or_router(sm, p_node, p_madw);
		break;

	case IB_NODE_TYPE_SWITCH:
		ni_rcv_process_existing_switch(sm, p_node, p_madw);
		break;

	default:
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D09: "
			"Unknown node type %u with GUID 0x%" PRIx64 "\n",
			p_ni->node_type, cl_ntoh64(p_ni->node_guid));
		break;
	}

	if ( p_ni->sys_guid != p_node->node_info.sys_guid) {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG, "Updated SysImageGUID: 0x%"
			PRIx64 " for node 0x%" PRIx64 "\n",
			cl_ntoh64(p_ni->sys_guid),
			cl_ntoh64(p_ni->node_guid));
	}
	ni_rcv_set_links(sm, p_node, port_num, p_ni_context);
	p_node->node_info = *p_ni;

Exit:
	OSM_LOG_EXIT(sm->p_log);
}