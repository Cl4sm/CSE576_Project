static int lid_mgr_set_physp_pi(IN osm_lid_mgr_t * p_mgr,
				IN osm_port_t * p_port,
				IN osm_physp_t * p_physp, IN ib_net16_t lid)
{
	uint8_t payload[IB_SMP_DATA_SIZE];
	ib_port_info_t *p_pi = (ib_port_info_t *) payload;
	const ib_port_info_t *p_old_pi;
	osm_madw_context_t context;
	osm_node_t *p_node;
	ib_api_status_t status;
	uint8_t mtu;
	uint8_t op_vls;
	uint8_t port_num;
	boolean_t send_set = FALSE;
	boolean_t update_mkey = FALSE;
	int ret = 0;

	OSM_LOG_ENTER(p_mgr->p_log);

	/*
	   Don't bother doing anything if this Physical Port is not valid.
	   This allows simplified code in the caller.
	 */
	if (!p_physp)
		goto Exit;

	port_num = osm_physp_get_port_num(p_physp);
	p_node = osm_physp_get_node_ptr(p_physp);

	if (osm_node_get_type(p_node) == IB_NODE_TYPE_SWITCH && port_num != 0) {
		/*
		   Switch ports that are not numbered 0 should not be set
		   with the following attributes as they are set later
		   (during NO_CHANGE state in link mgr).
		 */
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Skipping switch port %u, GUID 0x%016" PRIx64 "\n",
			port_num, cl_ntoh64(osm_physp_get_port_guid(p_physp)));
		goto Exit;
	}

	p_old_pi = &p_physp->port_info;

	/*
	   First, copy existing parameters from the PortInfo attribute we
	   already have for this node.

	   Second, update with default values that we know must be set for
	   every Physical Port and the LID and set the neighbor MTU field
	   appropriately.

	   Third, send the SMP to this physical port.
	 */

	memcpy(payload, p_old_pi, sizeof(ib_port_info_t));

	/*
	   Should never write back a value that is bigger then 3 in
	   the PortPhysicalState field, so cannot simply copy!

	   Actually we want to write there:
	   port physical state - no change
	   link down default state = polling
	   port state - no change
	 */
	p_pi->state_info2 = 0x02;
	ib_port_info_set_port_state(p_pi, IB_LINK_NO_CHANGE);

	if (ib_port_info_get_link_down_def_state(p_pi) !=
	    ib_port_info_get_link_down_def_state(p_old_pi))
		send_set = TRUE;

	/* didn't get PortInfo before */
	if (!ib_port_info_get_port_state(p_old_pi))
		send_set = TRUE;

	p_pi->m_key = p_mgr->p_subn->opt.m_key;
	if (memcmp(&p_pi->m_key, &p_old_pi->m_key, sizeof(p_pi->m_key))) {
		update_mkey = TRUE;
		send_set = TRUE;
	}

	p_pi->subnet_prefix = p_mgr->p_subn->opt.subnet_prefix;
	if (memcmp(&p_pi->subnet_prefix, &p_old_pi->subnet_prefix,
		   sizeof(p_pi->subnet_prefix)))
		send_set = TRUE;

	p_port->lid = lid;
	p_pi->base_lid = lid;
	if (memcmp(&p_pi->base_lid, &p_old_pi->base_lid,
		   sizeof(p_pi->base_lid))) {
		/*
		 * Reset stored base_lid.
		 * On successful send, we'll update it when we'll get a reply.
		 */
		osm_physp_set_base_lid(p_physp, 0);
		send_set = TRUE;
		p_mgr->dirty = TRUE;
	}

	/* we are updating the ports with our local sm_base_lid */
	p_pi->master_sm_base_lid = p_mgr->p_subn->sm_base_lid;
	if (memcmp(&p_pi->master_sm_base_lid, &p_old_pi->master_sm_base_lid,
		   sizeof(p_pi->master_sm_base_lid)))
		send_set = TRUE;

	p_pi->m_key_lease_period = p_mgr->p_subn->opt.m_key_lease_period;
	if (memcmp(&p_pi->m_key_lease_period, &p_old_pi->m_key_lease_period,
		   sizeof(p_pi->m_key_lease_period)))
		send_set = TRUE;

	p_pi->mkey_lmc = 0;
	ib_port_info_set_mpb(p_pi, p_mgr->p_subn->opt.m_key_protect_bits);
	if (ib_port_info_get_mpb(p_pi) != ib_port_info_get_mpb(p_old_pi))
		send_set = TRUE;

	/*
	   we want to set the timeout for both the switch port 0
	   and the CA ports
	 */
	ib_port_info_set_timeout(p_pi, p_mgr->p_subn->opt.subnet_timeout);
	if (ib_port_info_get_timeout(p_pi) !=
	    ib_port_info_get_timeout(p_old_pi))
		send_set = TRUE;

	if (port_num != 0) {
		/*
		   CAs don't have a port 0, and for switch port 0,
		   the state bits are ignored.
		   This is not the switch management port
		 */
		p_pi->link_width_enabled = p_old_pi->link_width_supported;
		if (memcmp(&p_pi->link_width_enabled,
			   &p_old_pi->link_width_enabled,
			   sizeof(p_pi->link_width_enabled)))
			send_set = TRUE;

		/* p_pi->mkey_lmc is initialized earlier */
		ib_port_info_set_lmc(p_pi, p_mgr->p_subn->opt.lmc);
		if (ib_port_info_get_lmc(p_pi) !=
		    ib_port_info_get_lmc(p_old_pi))
			send_set = TRUE;

		/* calc new op_vls and mtu */
		op_vls = osm_physp_calc_link_op_vls(p_mgr->p_log, p_mgr->p_subn,
					      p_physp,
					      ib_port_info_get_op_vls(p_old_pi));
		mtu = osm_physp_calc_link_mtu(p_mgr->p_log, p_physp,
					      ib_port_info_get_neighbor_mtu(p_old_pi));

		ib_port_info_set_neighbor_mtu(p_pi, mtu);

		if (ib_port_info_get_neighbor_mtu(p_pi) !=
		    ib_port_info_get_neighbor_mtu(p_old_pi))
			send_set = TRUE;

		ib_port_info_set_op_vls(p_pi, op_vls);
		if (ib_port_info_get_op_vls(p_pi) !=
		    ib_port_info_get_op_vls(p_old_pi))
			send_set = TRUE;

		/*
		   Several timeout mechanisms:
		 */
		ib_port_info_set_phy_and_overrun_err_thd(p_pi,
							 p_mgr->p_subn->opt.
							 local_phy_errors_threshold,
							 p_mgr->p_subn->opt.
							 overrun_errors_threshold);

		if (memcmp(&p_pi->error_threshold, &p_old_pi->error_threshold,
			   sizeof(p_pi->error_threshold)))
			send_set = TRUE;

		/*
		   To reset the port state machine we can send
		   PortInfo.State = DOWN. (see: 7.2.7 p171 lines:10-19)
		 */
		if (mtu != ib_port_info_get_neighbor_mtu(p_old_pi) ||
		    op_vls != ib_port_info_get_op_vls(p_old_pi)) {
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"Sending Link Down to GUID 0x%016"
				PRIx64 " port %d due to op_vls or "
				"mtu change. MTU:%u,%u VL_CAP:%u,%u\n",
				cl_ntoh64(osm_physp_get_port_guid(p_physp)),
				port_num, mtu,
				ib_port_info_get_neighbor_mtu(p_old_pi),
				op_vls, ib_port_info_get_op_vls(p_old_pi));

			/*
			   we need to make sure the internal DB will follow the
			   fact that the remote port is also going through
			   "down" state into "init"...
			 */
			lid_mgr_set_remote_pi_state_to_init(p_mgr, p_physp);

			ib_port_info_set_port_state(p_pi, IB_LINK_DOWN);
			if (ib_port_info_get_port_state(p_pi) !=
			    ib_port_info_get_port_state(p_old_pi))
				send_set = TRUE;
		}
	} else if (ib_switch_info_is_enhanced_port0(&p_node->sw->switch_info)) {
		/*
		 * Configure Enh. SP0:
		 * Set MTU according to the mtu_cap.
		 * Set LMC if lmc_esp0 is defined.
		 */
		ib_port_info_set_neighbor_mtu(p_pi,
					      ib_port_info_get_mtu_cap
					      (p_old_pi));
		if (ib_port_info_get_neighbor_mtu(p_pi) !=
		    ib_port_info_get_neighbor_mtu(p_old_pi))
			send_set = TRUE;

		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Updating neighbor_mtu on switch GUID 0x%016" PRIx64
			" port 0 to:%u\n",
			cl_ntoh64(osm_physp_get_port_guid(p_physp)),
			ib_port_info_get_neighbor_mtu(p_pi));

		/* Configure LMC on enhanced SP0 */
		if (p_mgr->p_subn->opt.lmc_esp0) {
			/* p_pi->mkey_lmc is initialized earlier */
			ib_port_info_set_lmc(p_pi, p_mgr->p_subn->opt.lmc);
			if (ib_port_info_get_lmc(p_pi) !=
			    ib_port_info_get_lmc(p_old_pi))
				send_set = TRUE;
		}
	}

	context.pi_context.node_guid = osm_node_get_node_guid(p_node);
	context.pi_context.port_guid = osm_physp_get_port_guid(p_physp);
	context.pi_context.set_method = TRUE;
	context.pi_context.light_sweep = FALSE;
	context.pi_context.active_transition = FALSE;

	/*
	   We need to set the cli_rereg bit when we are in first_time_master_sweep
	   for ports supporting the ClientReregistration Vol1 (v1.2) p811 14.4.11
	   Also, if this port was just now discovered, then we should also set
	   the cli_rereg bit. We know that the port was just discovered if its
	   is_new field is set.
	 */
	if ((p_mgr->p_subn->first_time_master_sweep == TRUE || p_port->is_new)
	    && !p_mgr->p_subn->opt.no_clients_rereg
	    && (p_old_pi->capability_mask & IB_PORT_CAP_HAS_CLIENT_REREG)) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Setting client rereg on %s, port %d\n",
			p_port->p_node->print_desc, p_port->p_physp->port_num);
		ib_port_info_set_client_rereg(p_pi, 1);
		context.pi_context.client_rereg = TRUE;
		send_set = TRUE;
	} else {
		ib_port_info_set_client_rereg(p_pi, 0);
		context.pi_context.client_rereg = FALSE;
	}

	/* We need to send the PortInfo Set request with the new sm_lid
	   in the following cases:
	   1. There is a change in the values (send_set == TRUE)
	   2. first_time_master_sweep flag on the subnet is TRUE. This means the
	   SM just became master, and it then needs to send a PortInfo Set to
	   every port.
	 */
	if (p_mgr->p_subn->first_time_master_sweep == TRUE)
		send_set = TRUE;

	if (!send_set)
		goto Exit;

	status = osm_req_set(p_mgr->sm, osm_physp_get_dr_path_ptr(p_physp),
			     payload, sizeof(payload), IB_MAD_ATTR_PORT_INFO,
			     cl_hton32(osm_physp_get_port_num(p_physp)),
			     FALSE, ib_port_info_get_m_key(&p_physp->port_info),
			     CL_DISP_MSGID_NONE, &context);
	if (status != IB_SUCCESS)
		ret = -1;
	/* If we sent a new mkey above, update our guid2mkey map
	   now, on the assumption that the SubnSet succeeds
	*/
	if (update_mkey)
		osm_db_guid2mkey_set(p_mgr->p_subn->p_g2m,
				     cl_ntoh64(p_physp->port_guid),
				     cl_ntoh64(p_pi->m_key));

Exit:
	OSM_LOG_EXIT(p_mgr->p_log);
	return ret;
}