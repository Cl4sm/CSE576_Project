void osm_pi_rcv_process(IN void *context, IN void *data)
{
	osm_sm_t *sm = context;
	osm_madw_t *p_madw = data;
	ib_port_info_t *p_pi;
	ib_smp_t *p_smp;
	osm_port_t *p_port;
	osm_physp_t *p_physp;
	osm_dr_path_t *p_dr_path;
	osm_node_t *p_node;
	osm_pi_context_t *p_context;
	ib_net64_t port_guid, node_guid;
	uint8_t port_num;

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(sm);
	CL_ASSERT(p_madw);

	p_smp = osm_madw_get_smp_ptr(p_madw);
	p_context = osm_madw_get_pi_context_ptr(p_madw);
	p_pi = ib_smp_get_payload_ptr(p_smp);

	CL_ASSERT(p_smp->attr_id == IB_MAD_ATTR_PORT_INFO);

	/*
	 * Attribute modifier has already been validated upon MAD receive,
	 * which means that port_num has to be valid - it originated from
	 * the request attribute modifier.
	 */
	port_num = (uint8_t) cl_ntoh32(p_smp->attr_mod);

	port_guid = p_context->port_guid;
	node_guid = p_context->node_guid;

	osm_dump_port_info_v2(sm->p_log, node_guid, port_guid, port_num, p_pi,
			      FILE_ID, OSM_LOG_DEBUG);

	/* On receipt of client reregister, clear the reregister bit so
	   reregistering won't be sent again and again */
	if (p_context->set_method &&
	    (ib_port_info_get_client_rereg(p_pi) || p_context->client_rereg)) {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Client reregister received on response\n");
		ib_port_info_set_client_rereg(p_pi, 0);
		p_context->client_rereg = FALSE;
	}

	/*
	   we might get a response during a light sweep looking for a change in
	   the status of a remote port that did not respond in earlier sweeps.
	   So if the context of the Get was light_sweep - we do not need to
	   do anything with the response - just flag that we need a heavy sweep
	 */
	if (p_context->light_sweep == TRUE) {
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Got light sweep response from remote port of parent node "
			"GUID 0x%" PRIx64 " port 0x%016" PRIx64
			", Commencing heavy sweep\n",
			cl_ntoh64(node_guid), cl_ntoh64(port_guid));
		sm->p_subn->force_heavy_sweep = TRUE;
		sm->p_subn->ignore_existing_lfts = TRUE;
		goto Exit;
	}

	CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);
	p_port = osm_get_port_by_guid(sm->p_subn, port_guid);
	if (PF(!p_port)) {
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

	if (PF(p_pi->local_port_num > p_node->node_info.num_ports)) {
		CL_PLOCK_RELEASE(sm->p_lock);
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0F15: "
			"Received PortInfo for port GUID 0x%" PRIx64 " is "
			"non-compliant and is being ignored since the "
			"local port num %u > num ports %u\n",
			cl_ntoh64(port_guid), p_pi->local_port_num,
			p_node->node_info.num_ports);
		goto Exit;
	}

	/*
	   If we were setting the PortInfo, then receiving
	   this attribute was not part of sweeping the subnet.
	   In this case, just update the PortInfo attribute.

	   In an unfortunate blunder, the IB spec defines the
	   return method for Set() as a GetResp().  Thus, we can't
	   use the method (what would have been SetResp()) to determine
	   our course of action.  So, we have to carry this extra
	   boolean around to determine if we were doing Get() or Set().
	 */
	if (p_context->set_method)
		pi_rcv_process_set(sm, p_node, port_num, p_madw);
	else {

		/*
		   This PortInfo arrived because we did a Get() method,
		   most likely due to a subnet sweep in progress.
		 */
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Discovered port num %u with GUID 0x%" PRIx64
			" for parent node GUID 0x%" PRIx64
			", TID 0x%" PRIx64 "\n",
			port_num, cl_ntoh64(port_guid),
			cl_ntoh64(node_guid), cl_ntoh64(p_smp->trans_id));

		p_physp = osm_node_get_physp_ptr(p_node, port_num);

		CL_ASSERT(p_physp);

		/* Update the directed route path to this port
		   in case the old path is no longer usable. */
		p_dr_path = osm_physp_get_dr_path_ptr(p_physp);
		osm_dr_path_init(p_dr_path, p_smp->hop_count,
				 p_smp->initial_path);

		p_physp->need_update = osm_pi_rcv_update_self(sm, p_physp, p_pi);

		switch (osm_node_get_type(p_node)) {
		case IB_NODE_TYPE_CA:
		case IB_NODE_TYPE_ROUTER:
			if (!p_node->physp_discovered[port_num]) {
				p_port->discovery_count++;
				p_node->physp_discovered[port_num] = 1;
			}
			p_physp->need_update = osm_pi_rcv_update_neighbor(p_physp);
			pi_rcv_process_ca_or_router_port(sm, p_node, p_physp,
							 p_pi);
			break;
		case IB_NODE_TYPE_SWITCH:
			if (!p_node->physp_discovered[port_num]) {
				p_port->discovery_count++;
				p_node->physp_discovered[port_num] = 1;
			}
			if (port_num == 0)
				pi_rcv_process_switch_port0(sm, p_node,
							    p_physp, p_pi);
			else
				pi_rcv_process_switch_ext_port(sm, p_node,
							       p_physp, p_pi);
			break;
		default:
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0F07: "
				"Unknown node type %u with GUID 0x%" PRIx64
				"\n", osm_node_get_type(p_node),
				cl_ntoh64(node_guid));
			break;
		}

		/*
		   Get the tables on the physp.
		 */
		if (p_physp->need_update || (p_node->sw &&
					     p_node->sw->need_update))
			pi_rcv_get_pkey_slvl_vla_tables(sm, p_node, p_physp);

	}

	CL_PLOCK_RELEASE(sm->p_lock);

Exit:
	/*
	   Release the lock before jumping here!!
	 */
	OSM_LOG_EXIT(sm->p_log);
}