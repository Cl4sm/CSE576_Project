static ib_api_status_t pr_rcv_get_path_parms(IN osm_sa_t * sa,
					     IN const ib_path_rec_t * p_pr,
					     IN const osm_alias_guid_t * p_src_alias_guid,
					     IN const uint16_t src_lid_ho,
					     IN const osm_alias_guid_t * p_dest_alias_guid,
					     IN const uint16_t dest_lid_ho,
					     IN const ib_net64_t comp_mask,
					     OUT osm_path_parms_t * p_parms)
{
	const osm_node_t *p_node;
	const osm_physp_t *p_physp, *p_physp0;
	const osm_physp_t *p_src_physp;
	const osm_physp_t *p_dest_physp;
	const osm_prtn_t *p_prtn = NULL;
	osm_opensm_t *p_osm;
	struct osm_routing_engine *p_re;
	const ib_port_info_t *p_pi, *p_pi0;
	ib_api_status_t status = IB_SUCCESS;
	ib_net16_t pkey;
	uint8_t mtu;
	uint8_t rate, p0_extended_rate, dest_rate;
	uint8_t pkt_life;
	uint8_t required_mtu;
	uint8_t required_rate;
	uint8_t required_pkt_life;
	uint8_t sl;
	uint8_t in_port_num;
	ib_net16_t dest_lid;
	uint8_t i;
	ib_slvl_table_t *p_slvl_tbl = NULL;
	osm_qos_level_t *p_qos_level = NULL;
	uint16_t valid_sl_mask = 0xffff;
	int hops = 0;
	int extended, p0_extended;

	OSM_LOG_ENTER(sa->p_log);

	dest_lid = cl_hton16(dest_lid_ho);

	p_dest_physp = p_dest_alias_guid->p_base_port->p_physp;
	p_physp = p_src_alias_guid->p_base_port->p_physp;
	p_src_physp = p_physp;
	p_pi = &p_physp->port_info;
	p_osm = sa->p_subn->p_osm;
	p_re = p_osm->routing_engine_used;

	mtu = ib_port_info_get_mtu_cap(p_pi);
	extended = p_pi->capability_mask & IB_PORT_CAP_HAS_EXT_SPEEDS;
	rate = ib_port_info_compute_rate(p_pi, extended);

	/*
	   Mellanox Tavor device performance is better using 1K MTU.
	   If required MTU and MTU selector are such that 1K is OK
	   and at least one end of the path is Tavor we override the
	   port MTU with 1K.
	 */
	if (sa->p_subn->opt.enable_quirks &&
	    sa_path_rec_apply_tavor_mtu_limit(p_pr,
					      p_src_alias_guid->p_base_port,
					      p_dest_alias_guid->p_base_port,
					      comp_mask))
		if (mtu > IB_MTU_LEN_1024) {
			mtu = IB_MTU_LEN_1024;
			OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
				"Optimized Path MTU to 1K for Mellanox Tavor device\n");
		}

	/*
	   Walk the subnet object from source to destination,
	   tracking the most restrictive rate and mtu values along the way...

	   If source port node is a switch, then p_physp should
	   point to the port that routes the destination lid
	 */

	p_node = osm_physp_get_node_ptr(p_physp);

	if (p_node->sw) {
		/*
		 * Source node is a switch.
		 * Make sure that p_physp points to the out port of the
		 * switch that routes to the destination lid (dest_lid_ho)
		 */
		p_physp = osm_switch_get_route_by_lid(p_node->sw, dest_lid);
		if (p_physp == 0) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F02: "
				"Cannot find routing to LID %u on switch "
				"%s (GUID: 0x%016" PRIx64 ")\n", dest_lid_ho,
				p_node->print_desc,
				cl_ntoh64(osm_node_get_node_guid(p_node)));
			status = IB_NOT_FOUND;
			goto Exit;
		}
	}

	if (sa->p_subn->opt.qos) {
		/*
		 * Whether this node is switch or CA, the IN port for
		 * the sl2vl table is 0, because this is a source node.
		 */
		p_slvl_tbl = osm_physp_get_slvl_tbl(p_physp, 0);

		/* update valid SLs that still exist on this route */
		for (i = 0; i < IB_MAX_NUM_VLS; i++) {
			if (valid_sl_mask & (1 << i) &&
			    ib_slvl_table_get(p_slvl_tbl, i) == IB_DROP_VL)
				valid_sl_mask &= ~(1 << i);
		}
		if (!valid_sl_mask) {
			OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
				"All the SLs lead to VL15 on this path\n");
			status = IB_NOT_FOUND;
			goto Exit;
		}
	}

	/*
	 * Same as above
	 */
	p_node = osm_physp_get_node_ptr(p_dest_physp);

	if (p_node->sw) {
		/*
		 * if destination is switch, we want p_dest_physp to point to port 0
		 */
		p_dest_physp =
		    osm_switch_get_route_by_lid(p_node->sw, dest_lid);

		if (p_dest_physp == 0) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F03: "
				"Cannot find routing to LID %u on switch "
				"%s (GUID: 0x%016" PRIx64 ")\n", dest_lid_ho,
				p_node->print_desc,
				cl_ntoh64(osm_node_get_node_guid(p_node)));
			status = IB_NOT_FOUND;
			goto Exit;
		}

	}

	/*
	 * Now go through the path step by step
	 */

	while (p_physp != p_dest_physp) {

		int tmp_pnum = p_physp->port_num;
		p_node = osm_physp_get_node_ptr(p_physp);
		p_physp = osm_physp_get_remote(p_physp);

		if (p_physp == 0) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F05: "
				"Can't find remote phys port of %s (GUID: "
				"0x%016"PRIx64") port %d "
				"while routing to LID %u\n",
				p_node->print_desc,
				cl_ntoh64(osm_node_get_node_guid(p_node)),
				tmp_pnum,
				dest_lid_ho);
			status = IB_ERROR;
			goto Exit;
		}

		in_port_num = osm_physp_get_port_num(p_physp);

		/*
		   This is point to point case (no switch in between)
		 */
		if (p_physp == p_dest_physp)
			break;

		p_node = osm_physp_get_node_ptr(p_physp);

		if (!p_node->sw) {
			/*
			   There is some sort of problem in the subnet object!
			   If this isn't a switch, we should have reached
			   the destination by now!
			 */
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F06: "
				"Internal error, bad path while routing "
				"%s (GUID: 0x%016"PRIx64") port %d to "
				"%s (GUID: 0x%016"PRIx64") port %d; "
				"ended at %s port %d\n",
				p_src_alias_guid->p_base_port->p_node->print_desc,
				cl_ntoh64(p_src_alias_guid->p_base_port->p_node->node_info.node_guid),
				p_src_alias_guid->p_base_port->p_physp->port_num,
				p_dest_alias_guid->p_base_port->p_node->print_desc,
				cl_ntoh64(p_dest_alias_guid->p_base_port->p_node->node_info.node_guid),
				p_dest_alias_guid->p_base_port->p_physp->port_num,
				p_node->print_desc,
				p_physp->port_num);
			status = IB_ERROR;
			goto Exit;
		}

		/*
		   Check parameters for the ingress port in this switch.
		 */
		p_pi = &p_physp->port_info;

		if (mtu > ib_port_info_get_mtu_cap(p_pi))
			mtu = ib_port_info_get_mtu_cap(p_pi);

		p_physp0 = osm_node_get_physp_ptr((osm_node_t *)p_node, 0);
		p_pi0 = &p_physp0->port_info;
		p0_extended = p_pi0->capability_mask & IB_PORT_CAP_HAS_EXT_SPEEDS;
		p0_extended_rate = ib_port_info_compute_rate(p_pi, p0_extended);
		if (ib_path_compare_rates(rate, p0_extended_rate) > 0)
			rate = p0_extended_rate;

		/*
		   Continue with the egress port on this switch.
		 */
		p_physp = osm_switch_get_route_by_lid(p_node->sw, dest_lid);
		if (p_physp == 0) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F07: "
				"Dead end path on switch "
				"%s (GUID: 0x%016"PRIx64") to LID %u\n",
				p_node->print_desc,
				cl_ntoh64(osm_node_get_node_guid(p_node)),
				dest_lid_ho);
			status = IB_ERROR;
			goto Exit;
		}

		p_pi = &p_physp->port_info;

		if (mtu > ib_port_info_get_mtu_cap(p_pi))
			mtu = ib_port_info_get_mtu_cap(p_pi);

		p_physp0 = osm_node_get_physp_ptr((osm_node_t *)p_node, 0);
		p_pi0 = &p_physp0->port_info;
		p0_extended = p_pi0->capability_mask & IB_PORT_CAP_HAS_EXT_SPEEDS;
		p0_extended_rate = ib_port_info_compute_rate(p_pi, p0_extended);
		if (ib_path_compare_rates(rate, p0_extended_rate) > 0)
			rate = p0_extended_rate;

		if (sa->p_subn->opt.qos) {
			/*
			 * Check SL2VL table of the switch and update valid SLs
			 */
			p_slvl_tbl =
			    osm_physp_get_slvl_tbl(p_physp, in_port_num);
			for (i = 0; i < IB_MAX_NUM_VLS; i++) {
				if (valid_sl_mask & (1 << i) &&
				    ib_slvl_table_get(p_slvl_tbl,
						      i) == IB_DROP_VL)
					valid_sl_mask &= ~(1 << i);
			}
			if (!valid_sl_mask) {
				OSM_LOG(sa->p_log, OSM_LOG_DEBUG, "All the SLs "
					"lead to VL15 on this path\n");
				status = IB_NOT_FOUND;
				goto Exit;
			}
		}

		/* update number of hops traversed */
		hops++;
		if (hops > MAX_HOPS) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F25: "
				"Path from GUID 0x%016" PRIx64 " (%s port %d) "
				"to lid %u GUID 0x%016" PRIx64 " (%s port %d) "
				"needs more than %d hops, max %d hops allowed\n",
				cl_ntoh64(osm_physp_get_port_guid(p_src_physp)),
				p_src_physp->p_node->print_desc,
				p_src_physp->port_num,
				dest_lid_ho,
				cl_ntoh64(osm_physp_get_port_guid
					  (p_dest_physp)),
				p_dest_physp->p_node->print_desc,
				p_dest_physp->port_num,
				hops,
				MAX_HOPS);
			status = IB_NOT_FOUND;
			goto Exit;
		}
	}

	/*
	   p_physp now points to the destination
	 */
	p_pi = &p_physp->port_info;

	if (mtu > ib_port_info_get_mtu_cap(p_pi))
		mtu = ib_port_info_get_mtu_cap(p_pi);

	extended = p_pi->capability_mask & IB_PORT_CAP_HAS_EXT_SPEEDS;
	dest_rate = ib_port_info_compute_rate(p_pi, extended);
	if (ib_path_compare_rates(rate, dest_rate) > 0)
		rate = dest_rate;

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"Path min MTU = %u, min rate = %u\n", mtu, rate);

	/*
	 * Get QoS Level object according to the path request
	 * and adjust path parameters according to QoS settings
	 */
	if (sa->p_subn->opt.qos &&
	    sa->p_subn->p_qos_policy &&
	    (p_qos_level =
	     osm_qos_policy_get_qos_level_by_pr(sa->p_subn->p_qos_policy,
						p_pr, p_src_physp, p_dest_physp,
						comp_mask))) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"PathRecord request matches QoS Level '%s' (%s)\n",
			p_qos_level->name, p_qos_level->use ?
			p_qos_level->use : "no description");

		if (p_qos_level->mtu_limit_set
		    && (mtu > p_qos_level->mtu_limit))
			mtu = p_qos_level->mtu_limit;

		if (p_qos_level->rate_limit_set
		    && (ib_path_compare_rates(rate, p_qos_level->rate_limit) > 0))
			rate = p_qos_level->rate_limit;

		if (p_qos_level->sl_set) {
			sl = p_qos_level->sl;
			if (!(valid_sl_mask & (1 << sl))) {
				status = IB_NOT_FOUND;
				goto Exit;
			}
		}
	}

	/*
	 * Set packet lifetime.
	 * According to spec definition IBA 1.2 Table 205
	 * PacketLifeTime description, for loopback paths,
	 * packetLifeTime shall be zero.
	 */
	if (p_src_alias_guid->p_base_port == p_dest_alias_guid->p_base_port)
		pkt_life = 0;
	else if (p_qos_level && p_qos_level->pkt_life_set)
		pkt_life = p_qos_level->pkt_life;
	else
		pkt_life = sa->p_subn->opt.subnet_timeout;

	/*
	   Determine if these values meet the user criteria
	   and adjust appropriately
	 */

	/* we silently ignore cases where only the MTU selector is defined */
	if ((comp_mask & IB_PR_COMPMASK_MTUSELEC) &&
	    (comp_mask & IB_PR_COMPMASK_MTU)) {
		required_mtu = ib_path_rec_mtu(p_pr);
		switch (ib_path_rec_mtu_sel(p_pr)) {
		case 0:	/* must be greater than */
			if (mtu <= required_mtu)
				status = IB_NOT_FOUND;
			break;

		case 1:	/* must be less than */
			if (mtu >= required_mtu) {
				/* adjust to use the highest mtu
				   lower than the required one */
				if (required_mtu > 1)
					mtu = required_mtu - 1;
				else
					status = IB_NOT_FOUND;
			}
			break;

		case 2:	/* exact match */
			if (mtu < required_mtu)
				status = IB_NOT_FOUND;
			else
				mtu = required_mtu;
			break;

		case 3:	/* largest available */
			/* can't be disqualified by this one */
			break;

		default:
			/* if we're here, there's a bug in ib_path_rec_mtu_sel() */
			CL_ASSERT(FALSE);
			status = IB_ERROR;
			break;
		}
	}
	if (status != IB_SUCCESS)
		goto Exit;

	/* we silently ignore cases where only the Rate selector is defined */
	if ((comp_mask & IB_PR_COMPMASK_RATESELEC) &&
	    (comp_mask & IB_PR_COMPMASK_RATE)) {
		required_rate = ib_path_rec_rate(p_pr);
		switch (ib_path_rec_rate_sel(p_pr)) {
		case 0:	/* must be greater than */
			if (ib_path_compare_rates(rate, required_rate) <= 0)
				status = IB_NOT_FOUND;
			break;

		case 1:	/* must be less than */
			if (ib_path_compare_rates(rate, required_rate) >= 0) {
				/* adjust the rate to use the highest rate
				   lower than the required one */
				rate = ib_path_rate_get_prev(required_rate);
				if (!rate)
					status = IB_NOT_FOUND;
			}
			break;

		case 2:	/* exact match */
			if (ib_path_compare_rates(rate, required_rate))
				status = IB_NOT_FOUND;
			else
				rate = required_rate;
			break;

		case 3:	/* largest available */
			/* can't be disqualified by this one */
			break;

		default:
			/* if we're here, there's a bug in ib_path_rec_mtu_sel() */
			CL_ASSERT(FALSE);
			status = IB_ERROR;
			break;
		}
	}
	if (status != IB_SUCCESS)
		goto Exit;

	/* we silently ignore cases where only the PktLife selector is defined */
	if ((comp_mask & IB_PR_COMPMASK_PKTLIFETIMESELEC) &&
	    (comp_mask & IB_PR_COMPMASK_PKTLIFETIME)) {
		required_pkt_life = ib_path_rec_pkt_life(p_pr);
		switch (ib_path_rec_pkt_life_sel(p_pr)) {
		case 0:	/* must be greater than */
			if (pkt_life <= required_pkt_life)
				status = IB_NOT_FOUND;
			break;

		case 1:	/* must be less than */
			if (pkt_life >= required_pkt_life) {
				/* adjust the lifetime to use the highest possible
				   lower than the required one */
				if (required_pkt_life > 1)
					pkt_life = required_pkt_life - 1;
				else
					status = IB_NOT_FOUND;
			}
			break;

		case 2:	/* exact match */
			if (pkt_life < required_pkt_life)
				status = IB_NOT_FOUND;
			else
				pkt_life = required_pkt_life;
			break;

		case 3:	/* smallest available */
			/* can't be disqualified by this one */
			break;

		default:
			/* if we're here, there's a bug in ib_path_rec_pkt_life_sel() */
			CL_ASSERT(FALSE);
			status = IB_ERROR;
			break;
		}
	}

	if (status != IB_SUCCESS)
		goto Exit;

	/*
	 * set Pkey for this path record request
	 */

	if ((comp_mask & IB_PR_COMPMASK_RAWTRAFFIC) &&
	    (cl_ntoh32(p_pr->hop_flow_raw) & (1 << 31)))
		pkey = osm_physp_find_common_pkey(p_src_physp, p_dest_physp,
						  sa->p_subn->opt.allow_both_pkeys);

	else if (comp_mask & IB_PR_COMPMASK_PKEY) {
		/*
		 * PR request has a specific pkey:
		 * Check that source and destination share this pkey.
		 * If QoS level has pkeys, check that this pkey exists
		 * in the QoS level pkeys.
		 * PR returned pkey is the requested pkey.
		 */
		pkey = p_pr->pkey;
		if (!osm_physp_share_this_pkey(p_src_physp, p_dest_physp, pkey,
					       sa->p_subn->opt.allow_both_pkeys)) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F1A: "
				"Ports 0x%016" PRIx64 " (%s port %d) and "
				"0x%016" PRIx64 " (%s port %d) "
				"do not share specified PKey 0x%04x\n",
				cl_ntoh64(osm_physp_get_port_guid(p_src_physp)),
				p_src_physp->p_node->print_desc,
				p_src_physp->port_num,
				cl_ntoh64(osm_physp_get_port_guid
					  (p_dest_physp)),
				p_dest_physp->p_node->print_desc,
				p_dest_physp->port_num,
				cl_ntoh16(pkey));
			status = IB_NOT_FOUND;
			goto Exit;
		}
		if (p_qos_level && p_qos_level->pkey_range_len &&
		    !osm_qos_level_has_pkey(p_qos_level, pkey)) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F1D: "
				"QoS level \"%s\" doesn't define specified PKey 0x%04x "
				"for ports 0x%016" PRIx64 " (%s port %d) and "
				"0x%016"PRIx64" (%s port %d)\n",
				p_qos_level->name,
				cl_ntoh16(pkey),
				cl_ntoh64(osm_physp_get_port_guid(p_src_physp)),
				p_src_physp->p_node->print_desc,
				p_src_alias_guid->p_base_port->p_physp->port_num,
				cl_ntoh64(osm_physp_get_port_guid
					  (p_dest_physp)),
				p_dest_physp->p_node->print_desc,
				p_dest_alias_guid->p_base_port->p_physp->port_num);
			status = IB_NOT_FOUND;
			goto Exit;
		}

	} else if (p_qos_level && p_qos_level->pkey_range_len) {
		/*
		 * PR request doesn't have a specific pkey, but QoS level
		 * has pkeys - get shared pkey from QoS level pkeys
		 */
		pkey = osm_qos_level_get_shared_pkey(p_qos_level,
						     p_src_physp, p_dest_physp,
						     sa->p_subn->opt.allow_both_pkeys);
		if (!pkey) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F1E: "
				"Ports 0x%016" PRIx64 " (%s) and "
				"0x%016" PRIx64 " (%s) do not share "
				"PKeys defined by QoS level \"%s\"\n",
				cl_ntoh64(osm_physp_get_port_guid(p_src_physp)),
				p_src_physp->p_node->print_desc,
				cl_ntoh64(osm_physp_get_port_guid
					  (p_dest_physp)),
				p_dest_physp->p_node->print_desc,
				p_qos_level->name);
			status = IB_NOT_FOUND;
			goto Exit;
		}
	} else {
		/*
		 * Neither PR request nor QoS level have pkey.
		 * Just get any shared pkey.
		 */
		pkey = osm_physp_find_common_pkey(p_src_physp, p_dest_physp,
						  sa->p_subn->opt.allow_both_pkeys);
		if (!pkey) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F1B: "
				"Ports src 0x%016"PRIx64" (%s port %d) and "
				"dst 0x%016"PRIx64" (%s port %d) do not have "
				"any shared PKeys\n",
				cl_ntoh64(osm_physp_get_port_guid(p_src_physp)),
				p_src_physp->p_node->print_desc,
				p_src_physp->port_num,
				cl_ntoh64(osm_physp_get_port_guid
					  (p_dest_physp)),
				p_dest_physp->p_node->print_desc,
				p_dest_physp->port_num);
			status = IB_NOT_FOUND;
			goto Exit;
		}
	}

	if (pkey) {
		p_prtn =
		    (osm_prtn_t *) cl_qmap_get(&sa->p_subn->prtn_pkey_tbl,
					       pkey & cl_hton16((uint16_t) ~
								0x8000));
		if (p_prtn ==
		    (osm_prtn_t *) cl_qmap_end(&sa->p_subn->prtn_pkey_tbl))
			p_prtn = NULL;
	}

	/*
	 * Set PathRecord SL
	 */

	if (comp_mask & IB_PR_COMPMASK_SL) {
		/*
		 * Specific SL was requested
		 */
		sl = ib_path_rec_sl(p_pr);

		if (p_qos_level && p_qos_level->sl_set
		    && (p_qos_level->sl != sl)) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F1F: "
				"QoS constraints: required PathRecord SL (%u) "
				"doesn't match QoS policy \"%s\" SL (%u) "
				"[%s port %d <-> %s port %d]\n", sl,
				p_qos_level->name,
				p_qos_level->sl,
				p_src_alias_guid->p_base_port->p_node->print_desc,
				p_src_alias_guid->p_base_port->p_physp->port_num,
				p_dest_alias_guid->p_base_port->p_node->print_desc,
				p_dest_alias_guid->p_base_port->p_physp->port_num);
			status = IB_NOT_FOUND;
			goto Exit;
		}

	} else if (p_qos_level && p_qos_level->sl_set) {
		/*
		 * No specific SL was requested, but there is an SL in
		 * QoS level.
		 */
		sl = p_qos_level->sl;

		if (pkey && p_prtn && p_prtn->sl != p_qos_level->sl)
			OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
				"QoS level SL (%u) overrides partition SL (%u)\n",
				p_qos_level->sl, p_prtn->sl);

	} else if (pkey) {
		/*
		 * No specific SL in request or in QoS level - use partition SL
		 */
		if (!p_prtn) {
			sl = OSM_DEFAULT_SL;
			/* this may be possible when pkey tables are created somehow in
			   previous runs or things are going wrong here */
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F1C: "
				"No partition found for PKey 0x%04x - "
				"using default SL %d "
				"[%s port %d <-> %s port %d]\n",
				cl_ntoh16(pkey), sl,
				p_src_alias_guid->p_base_port->p_node->print_desc,
				p_src_alias_guid->p_base_port->p_physp->port_num,
				p_dest_alias_guid->p_base_port->p_node->print_desc,
				p_dest_alias_guid->p_base_port->p_physp->port_num);
		} else
			sl = p_prtn->sl;
	} else if (sa->p_subn->opt.qos) {
		if (valid_sl_mask & (1 << OSM_DEFAULT_SL))
			sl = OSM_DEFAULT_SL;
		else {
			for (i = 0; i < IB_MAX_NUM_VLS; i++)
				if (valid_sl_mask & (1 << i))
					break;
			sl = i;
		}
	} else
		sl = OSM_DEFAULT_SL;

	if (sa->p_subn->opt.qos && !(valid_sl_mask & (1 << sl))) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F24: "
			"Selected SL (%u) leads to VL15 "
			"[%s port %d <-> %s port %d]\n",
			sl,
			p_src_alias_guid->p_base_port->p_node->print_desc,
			p_src_alias_guid->p_base_port->p_physp->port_num,
			p_dest_alias_guid->p_base_port->p_node->print_desc,
			p_dest_alias_guid->p_base_port->p_physp->port_num);
		status = IB_NOT_FOUND;
		goto Exit;
	}

	/*
	 * If the routing engine wants to have a say in path SL selection,
	 * send the currently computed SL value as a hint and let the routing
	 * engine override it.
	 */
	if (p_re && p_re->path_sl) {
		uint8_t pr_sl;
		pr_sl = sl;

		sl = p_re->path_sl(p_re->context, sl,
				   cl_hton16(src_lid_ho), cl_hton16(dest_lid_ho));

		if ((comp_mask & IB_PR_COMPMASK_SL) && (sl != pr_sl)) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F2A: "
				"Requested SL (%u) doesn't match SL calculated"
				"by routing engine (%u) "
				"[%s port %d <-> %s port %d]\n",
				pr_sl,
				sl,
				p_src_alias_guid->p_base_port->p_node->print_desc,
				p_src_alias_guid->p_base_port->p_physp->port_num,
				p_dest_alias_guid->p_base_port->p_node->print_desc,
				p_dest_alias_guid->p_base_port->p_physp->port_num);
			status = IB_NOT_FOUND;
			goto Exit;
		}
	}
	/* reset pkey when raw traffic */
	if (comp_mask & IB_PR_COMPMASK_RAWTRAFFIC &&
	    cl_ntoh32(p_pr->hop_flow_raw) & (1 << 31))
		pkey = 0;

	p_parms->mtu = mtu;
	p_parms->rate = rate;
	p_parms->pkt_life = pkt_life;
	p_parms->pkey = pkey;
	p_parms->sl = sl;

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG, "Path params: mtu = %u, rate = %u,"
		" packet lifetime = %u, pkey = 0x%04X, sl = %u\n",
		mtu, rate, pkt_life, cl_ntoh16(pkey), sl);
Exit:
	OSM_LOG_EXIT(sa->p_log);
	return status;
}