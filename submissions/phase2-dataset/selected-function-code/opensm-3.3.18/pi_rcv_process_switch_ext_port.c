static void pi_rcv_process_switch_ext_port(IN osm_sm_t * sm,
					   IN osm_node_t * p_node,
					   IN osm_physp_t * p_physp,
					   IN ib_port_info_t * p_pi)
{
	ib_api_status_t status = IB_SUCCESS;
	osm_madw_context_t context;
	osm_physp_t *p_remote_physp, *physp0;
	osm_node_t *p_remote_node;
	ib_net64_t m_key;
	unsigned data_vls;
	uint8_t port_num;
	uint8_t remote_port_num;
	osm_dr_path_t path;
	int mlnx_epi_supported = 0;

	OSM_LOG_ENTER(sm->p_log);

	/*
	   Check the state of the physical port.
	   If there appears to be something on the other end of the wire,
	   then ask for NodeInfo.  Ignore the switch management port.
	 */
	port_num = osm_physp_get_port_num(p_physp);

	if (sm->p_subn->opt.fdr10)
		mlnx_epi_supported = is_mlnx_ext_port_info_supported(p_node->node_info.device_id);

	/* if in_sweep_hop_0 is TRUE, then this means the SM is on the switch,
	   and we got switchInfo of our local switch. Do not continue
	   probing through the switch. */
	if (sm->p_subn->in_sweep_hop_0 == FALSE) {
		switch (ib_port_info_get_port_state(p_pi)) {
		case IB_LINK_DOWN:
			p_remote_physp = osm_physp_get_remote(p_physp);
			if (p_remote_physp) {
				p_remote_node =
				    osm_physp_get_node_ptr(p_remote_physp);
				remote_port_num =
				    osm_physp_get_port_num(p_remote_physp);

				OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
					"Unlinking local node 0x%" PRIx64
					", port %u"
					"\n\t\t\t\tand remote node 0x%" PRIx64
					", port %u\n",
					cl_ntoh64(osm_node_get_node_guid
						  (p_node)), port_num,
					cl_ntoh64(osm_node_get_node_guid
						  (p_remote_node)),
					remote_port_num);

				if (sm->ucast_mgr.cache_valid)
					osm_ucast_cache_add_link(&sm->ucast_mgr,
								 p_physp,
								 p_remote_physp);

				osm_node_unlink(p_node, (uint8_t) port_num,
						p_remote_node,
						(uint8_t) remote_port_num);

			}
			break;

		case IB_LINK_INIT:
		case IB_LINK_ARMED:
		case IB_LINK_ACTIVE:
			physp0 = osm_node_get_physp_ptr(p_node, 0);
			if (mlnx_epi_supported) {
				m_key = ib_port_info_get_m_key(&physp0->port_info);

				context.pi_context.node_guid = osm_node_get_node_guid(p_node);
				context.pi_context.port_guid = osm_physp_get_port_guid(p_physp);
				context.pi_context.set_method = FALSE;
				context.pi_context.light_sweep = FALSE;
				context.pi_context.active_transition = FALSE;
				context.pi_context.client_rereg = FALSE;
				status = osm_req_get(sm,
						     osm_physp_get_dr_path_ptr(p_physp),
						     IB_MAD_ATTR_MLNX_EXTENDED_PORT_INFO,
						     cl_hton32(port_num), FALSE, m_key,
						     CL_DISP_MSGID_NONE, &context);
				if (status != IB_SUCCESS)
					OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0F11: "
						"Failure initiating MLNX ExtPortInfo request (%s)\n",
						ib_get_err_str(status));
		        }
			/*
			   To avoid looping forever, only probe the port if it
			   is NOT the port that responded to the SMP.

			   Request node info from the other end of this link:
			   1) Copy the current path from the parent node.
			   2) Extend the path to the next hop thru this port.
			   3) Request node info with the new path

			 */
			if (p_pi->local_port_num !=
			    osm_physp_get_port_num(p_physp)) {
				path = *osm_physp_get_dr_path_ptr(p_physp);

				if (osm_dr_path_extend(&path,
						       osm_physp_get_port_num
						       (p_physp))) {
					OSM_LOG(sm->p_log, OSM_LOG_ERROR,
						"ERR 0F08: "
						"DR path with hop count %d couldn't be extended\n",
						path.hop_count);
					break;
				}

				memset(&context, 0, sizeof(context));
				context.ni_context.node_guid =
				    osm_node_get_node_guid(p_node);
				context.ni_context.port_num =
				    osm_physp_get_port_num(p_physp);

				status = osm_req_get(sm, &path,
						     IB_MAD_ATTR_NODE_INFO, 0,
						     TRUE, 0,
						     CL_DISP_MSGID_NONE,
						     &context);

				if (status != IB_SUCCESS)
					OSM_LOG(sm->p_log, OSM_LOG_ERROR,
						"ERR 0F02: "
						"Failure initiating NodeInfo request (%s)\n",
						ib_get_err_str(status));
			} else
				OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
					"Skipping SMP responder port %u\n",
					p_pi->local_port_num);
			break;

		default:
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0F03: "
				"Unknown link state = %u, port = %u\n",
				ib_port_info_get_port_state(p_pi),
				p_pi->local_port_num);
			break;
		}
	}

	if (ib_port_info_get_port_state(p_pi) > IB_LINK_INIT && p_node->sw &&
	    !ib_switch_info_get_state_change(&p_node->sw->switch_info) &&
	    p_node->sw->need_update == 1)
		p_node->sw->need_update = 0;

	if (p_physp->need_update)
		sm->p_subn->ignore_existing_lfts = TRUE;

	/*
	   Update the PortInfo attribute.
	 */
	osm_physp_set_port_info(p_physp, p_pi, sm);

	if (ib_port_info_get_port_state(p_pi) == IB_LINK_DOWN)
		goto Exit;

	p_remote_physp = osm_physp_get_remote(p_physp);
	if (p_remote_physp) {
		p_remote_node = osm_physp_get_node_ptr(p_remote_physp);
		if (p_remote_node->sw) {
			data_vls = 1U << (ib_port_info_get_vl_cap(p_pi) - 1);
			if (data_vls > sm->p_subn->opt.max_op_vls)
				data_vls = sm->p_subn->opt.max_op_vls;
			if (data_vls >= IB_MAX_NUM_VLS)
				data_vls = IB_MAX_NUM_VLS - 1;
			if ((uint8_t)data_vls < sm->p_subn->min_sw_data_vls) {
				OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
					"Setting switch port minimal data VLs "
					"to:%u defined by node:0x%"
					PRIx64 ", port:%u\n", data_vls,
					cl_ntoh64(osm_node_get_node_guid(p_node)),
					port_num);
				sm->p_subn->min_sw_data_vls = data_vls;
			}
		}
	}

Exit:
	OSM_LOG_EXIT(sm->p_log);
}