void osm_mlnx_epi_rcv_process(IN void *context, IN void *data)
{
	osm_sm_t *sm = context;
	osm_madw_t *p_madw = data;
	ib_mlnx_ext_port_info_t *p_pi;
	ib_smp_t *p_smp;
	osm_port_t *p_port;
	osm_physp_t *p_physp;
	osm_node_t *p_node;
	osm_pi_context_t *p_context;
	ib_net64_t port_guid, node_guid;
	uint8_t port_num, portnum, start_port = 1;

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(sm);
	CL_ASSERT(p_madw);

	p_smp = osm_madw_get_smp_ptr(p_madw);
	p_context = osm_madw_get_pi_context_ptr(p_madw);
	p_pi = ib_smp_get_payload_ptr(p_smp);

	CL_ASSERT(p_smp->attr_id == IB_MAD_ATTR_MLNX_EXTENDED_PORT_INFO);

	port_num = (uint8_t) cl_ntoh32(p_smp->attr_mod);

	port_guid = p_context->port_guid;
	node_guid = p_context->node_guid;

	osm_dump_mlnx_ext_port_info_v2(sm->p_log, node_guid, port_guid, port_num,
				       p_pi, FILE_ID, OSM_LOG_DEBUG);

	CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);
	p_port = osm_get_port_by_guid(sm->p_subn, port_guid);
	if (!p_port) {
		CL_PLOCK_RELEASE(sm->p_lock);
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0F06: "
			"No port object for port with GUID 0x%" PRIx64
			"\n\t\t\t\tfor parent node GUID 0x%" PRIx64
			", TID 0x%" PRIx64 "\n",
			cl_ntoh64(port_guid),
			cl_ntoh64(node_guid), cl_ntoh64(p_smp->trans_id));
		goto Exit;
	}

	p_node = p_port->p_node;
	CL_ASSERT(p_node);

	if (!(cl_ntoh16(p_smp->status) & 0x7fff)) {
		if (port_num != 255) {
			p_physp = osm_node_get_physp_ptr(p_node, port_num);
			CL_ASSERT(p_physp);
			p_physp->ext_port_info = *p_pi;
		} else {
			/* Handle all ports on set/set resp */
			if (p_node->sw &&
			    ib_switch_info_is_enhanced_port0(&p_node->sw->switch_info))
				start_port = 0;

			for (portnum = start_port;
			     portnum < osm_node_get_num_physp(p_node);
			     portnum++) {
				p_physp = osm_node_get_physp_ptr(p_node, portnum);
				CL_ASSERT(p_physp);
				p_physp->ext_port_info = *p_pi;
			}
		}
	} else
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"MAD status 0x%x received\n",
			cl_ntoh16(p_smp->status) & 0x7fff);

	CL_PLOCK_RELEASE(sm->p_lock);

Exit:
	/*
	   Release the lock before jumping here!!
	 */
	OSM_LOG_EXIT(sm->p_log);
}