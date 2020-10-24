static ib_api_status_t state_mgr_sweep_hop_1(IN osm_sm_t * sm)
{
	ib_api_status_t status = IB_SUCCESS;
	osm_madw_context_t context;
	osm_node_t *p_node;
	osm_port_t *p_port;
	osm_dr_path_t hop_1_path;
	ib_net64_t port_guid;
	uint8_t port_num;
	uint8_t path_array[IB_SUBNET_PATH_HOPS_MAX];
	uint8_t num_ports;
	osm_physp_t *p_ext_physp;

	OSM_LOG_ENTER(sm->p_log);

	/*
	 * First, get our own port and node objects.
	 */
	port_guid = sm->p_subn->sm_port_guid;

	CL_ASSERT(port_guid);

	/* Set the in_sweep_hop_0 flag in subn to be FALSE.
	 * This will indicate the sweeping to continue beyond the
	 * the current node.
	 * This is relevant for the case of SM on switch, since in the
	 * switch info we need to signal that the sweeping should
	 * continue through the switch. */
	sm->p_subn->in_sweep_hop_0 = FALSE;

	p_port = osm_get_port_by_guid(sm->p_subn, port_guid);
	if (!p_port) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3310: "
			"No SM port object\n");
		status = IB_ERROR;
		goto Exit;
	}

	p_node = p_port->p_node;
	CL_ASSERT(p_node);

	port_num = ib_node_info_get_local_port_num(&p_node->node_info);

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
		"Probing hop 1 on local port %u\n", port_num);

	memset(path_array, 0, sizeof(path_array));
	/* the hop_1 operations depend on the type of our node.
	 * Currently - legal nodes that can host SM are SW and CA */
	switch (osm_node_get_type(p_node)) {
	case IB_NODE_TYPE_CA:
	case IB_NODE_TYPE_ROUTER:
		memset(&context, 0, sizeof(context));
		context.ni_context.node_guid = osm_node_get_node_guid(p_node);
		context.ni_context.port_num = port_num;

		path_array[1] = port_num;

		osm_dr_path_init(&hop_1_path, 1, path_array);
		CL_PLOCK_ACQUIRE(sm->p_lock);
		status = osm_req_get(sm, &hop_1_path, IB_MAD_ATTR_NODE_INFO, 0,
				     TRUE, 0, CL_DISP_MSGID_NONE, &context);
		CL_PLOCK_RELEASE(sm->p_lock);
		if (status != IB_SUCCESS)
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3311: "
				"Request for NodeInfo failed (%s)\n",
				ib_get_err_str(status));
		break;

	case IB_NODE_TYPE_SWITCH:
		/* Need to go over all the ports of the switch, and send a
		 * node_info from them. This doesn't include the port 0 of the
		 * switch, which hosts the SM.
		 * Note: We'll send another switchInfo on port 0, since if no
		 * ports are connected, we still want to get some response, and
		 * have the subnet come up.
		 */
		num_ports = osm_node_get_num_physp(p_node);
		for (port_num = 1; port_num < num_ports; port_num++) {
			/* go through the port only if the port is not DOWN */
			p_ext_physp = osm_node_get_physp_ptr(p_node, port_num);
			if (p_ext_physp && ib_port_info_get_port_state
			    (&(p_ext_physp->port_info)) > IB_LINK_DOWN) {
				memset(&context, 0, sizeof(context));
				context.ni_context.node_guid =
				    osm_node_get_node_guid(p_node);
				context.ni_context.port_num = port_num;

				path_array[1] = port_num;
				osm_dr_path_init(&hop_1_path, 1, path_array);
				CL_PLOCK_ACQUIRE(sm->p_lock);
				status = osm_req_get(sm, &hop_1_path,
						     IB_MAD_ATTR_NODE_INFO, 0,
						     TRUE, 0,
						     CL_DISP_MSGID_NONE,
						     &context);
				CL_PLOCK_RELEASE(sm->p_lock);
				if (status != IB_SUCCESS)
					OSM_LOG(sm->p_log, OSM_LOG_ERROR,
						"ERR 3312: "
						"Request for NodeInfo failed (%s)\n",
						ib_get_err_str(status));
			}
		}
		break;

	default:
		OSM_LOG(sm->p_log, OSM_LOG_ERROR,
			"ERR 3313: Unknown node type %d (%s)\n",
			osm_node_get_type(p_node), p_node->print_desc);
	}

Exit:
	OSM_LOG_EXIT(sm->p_log);
	return status;
}