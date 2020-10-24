static void mcmr_by_comp_mask(osm_sa_t * sa, const ib_member_rec_t * p_rcvd_rec,
			      ib_net64_t comp_mask, osm_mgrp_t * p_mgrp,
			      const osm_physp_t * p_req_physp,
			      boolean_t trusted_req, cl_qlist_t * list)
{
	/* since we might change scope_state */
	ib_member_rec_t match_rec;
	osm_mcm_alias_guid_t *p_mcm_alias_guid;
	ib_net64_t portguid = p_rcvd_rec->port_gid.unicast.interface_id;
	/* will be used for group or port info */
	uint8_t scope_state;
	uint8_t scope_state_mask = 0;
	cl_map_item_t *p_item;
	ib_gid_t port_gid;
	boolean_t proxy_join = FALSE;

	OSM_LOG_ENTER(sa->p_log);

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"Checking mlid:0x%X\n", cl_ntoh16(p_mgrp->mlid));

	/* first try to eliminate the group by MGID, MLID, or P_Key */
	if ((IB_MCR_COMPMASK_MGID & comp_mask) &&
	    memcmp(&p_rcvd_rec->mgid, &p_mgrp->mcmember_rec.mgid,
		   sizeof(ib_gid_t)))
		goto Exit;

	if ((IB_MCR_COMPMASK_MLID & comp_mask) &&
	    memcmp(&p_rcvd_rec->mlid, &p_mgrp->mcmember_rec.mlid,
		   sizeof(uint16_t)))
		goto Exit;

	/* if the requester physical port doesn't have the pkey that is defined
	   for the group - exit. */
	if (!osm_physp_has_pkey(sa->p_log, p_mgrp->mcmember_rec.pkey,
				p_req_physp))
		goto Exit;

	/* now do the rest of the match */
	if ((IB_MCR_COMPMASK_QKEY & comp_mask) &&
	    p_rcvd_rec->qkey != p_mgrp->mcmember_rec.qkey)
		goto Exit;

	if ((IB_MCR_COMPMASK_PKEY & comp_mask) &&
	    p_rcvd_rec->pkey != p_mgrp->mcmember_rec.pkey)
		goto Exit;

	if ((IB_MCR_COMPMASK_TCLASS & comp_mask) &&
	    p_rcvd_rec->tclass != p_mgrp->mcmember_rec.tclass)
		goto Exit;

	/* check SL, Flow, and Hop limit */
	{
		uint8_t mgrp_sl, query_sl;
		uint32_t mgrp_flow, query_flow;
		uint8_t mgrp_hop, query_hop;

		ib_member_get_sl_flow_hop(p_rcvd_rec->sl_flow_hop,
					  &query_sl, &query_flow, &query_hop);

		ib_member_get_sl_flow_hop(p_mgrp->mcmember_rec.sl_flow_hop,
					  &mgrp_sl, &mgrp_flow, &mgrp_hop);

		if ((IB_MCR_COMPMASK_SL & comp_mask) && query_sl != mgrp_sl)
			goto Exit;

		if ((IB_MCR_COMPMASK_FLOW & comp_mask) &&
		    query_flow != mgrp_flow)
			goto Exit;

		if ((IB_MCR_COMPMASK_HOP & comp_mask) && query_hop != mgrp_hop)
			goto Exit;
	}

	if ((IB_MCR_COMPMASK_PROXY & comp_mask) &&
	    p_rcvd_rec->proxy_join != p_mgrp->mcmember_rec.proxy_join)
		goto Exit;

	/* need to validate mtu, rate, and pkt_lifetime fields */
	if (validate_more_comp_fields(sa->p_log, p_mgrp, p_rcvd_rec,
				      comp_mask) == FALSE)
		goto Exit;

	/* Port specific fields */
	/* so did we get the PortGUID mask */
	if (IB_MCR_COMPMASK_PORT_GID & comp_mask) {
		/* try to find this port */
		p_mcm_alias_guid = osm_mgrp_get_mcm_alias_guid(p_mgrp, portguid);
		if (!p_mcm_alias_guid) /* port not in group */
			goto Exit;
		scope_state = p_mcm_alias_guid->scope_state;
		memcpy(&port_gid, &(p_mcm_alias_guid->port_gid),
		       sizeof(ib_gid_t));
		proxy_join = p_mcm_alias_guid->proxy_join;
	} else /* point to the group information */
		scope_state = p_mgrp->mcmember_rec.scope_state;

	if (IB_MCR_COMPMASK_SCOPE & comp_mask)
		scope_state_mask = 0xF0;

	if (IB_MCR_COMPMASK_JOIN_STATE & comp_mask)
		scope_state_mask = scope_state_mask | 0x0F;

	/* Many MC records returned */
	if (trusted_req == TRUE && !(IB_MCR_COMPMASK_PORT_GID & comp_mask)) {
		char gid_str[INET6_ADDRSTRLEN];
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Trusted req is TRUE and no specific port defined\n");

		/* return all the ports that match in this MC group */
		p_item = cl_qmap_head(&(p_mgrp->mcm_alias_port_tbl));
		while (p_item != cl_qmap_end(&(p_mgrp->mcm_alias_port_tbl))) {
			p_mcm_alias_guid = (osm_mcm_alias_guid_t *) p_item;

			if ((scope_state_mask & p_rcvd_rec->scope_state) ==
			    (scope_state_mask & p_mcm_alias_guid->scope_state)) {
				/* add to the list */
				match_rec = p_mgrp->mcmember_rec;
				match_rec.scope_state = p_mcm_alias_guid->scope_state;
				memcpy(&match_rec.port_gid,
				       &p_mcm_alias_guid->port_gid,
				       sizeof(ib_gid_t));
				OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
					"Record of port_gid: %s"
					" in multicast_lid: 0x%X is returned\n",
					inet_ntop(AF_INET6,
						  match_rec.port_gid.raw,
						  gid_str, sizeof gid_str),
					cl_ntoh16(p_mgrp->mlid));

				match_rec.proxy_join =
				    (uint8_t) (p_mcm_alias_guid->proxy_join);

				mcmr_rcv_new_mcmr(sa, &match_rec, list);
			}
			p_item = cl_qmap_next(p_item);
		}
	} else { /* One MC record returned */
		if ((scope_state_mask & p_rcvd_rec->scope_state) !=
		    (scope_state_mask & scope_state))
			goto Exit;

		/* add to the list */
		match_rec = p_mgrp->mcmember_rec;
		match_rec.scope_state = scope_state;
		memcpy(&(match_rec.port_gid), &port_gid, sizeof(ib_gid_t));
		match_rec.proxy_join = (uint8_t) proxy_join;

		mcmr_rcv_new_mcmr(sa, &match_rec, list);
	}

Exit:
	OSM_LOG_EXIT(sa->p_log);
}