static void mcmr_rcv_join_mgrp(IN osm_sa_t * sa, IN osm_madw_t * p_madw)
{
	osm_mgrp_t *p_mgrp = NULL;
	ib_api_status_t status;
	ib_sa_mad_t *p_sa_mad;
	ib_member_rec_t *p_recvd_mcmember_rec;
	ib_member_rec_t mcmember_rec;
	osm_mcm_port_t *p_mcmr_port;
	osm_mcm_alias_guid_t *p_mcm_alias_guid;
	ib_net64_t portguid;
	osm_port_t *p_port;
	osm_physp_t *p_physp;
	osm_physp_t *p_request_physp;
	uint8_t is_new_group;	/* TRUE = there is a need to create a group */
	uint8_t join_state;
	boolean_t proxy;

	OSM_LOG_ENTER(sa->p_log);

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_recvd_mcmember_rec = ib_sa_mad_get_payload_ptr(p_sa_mad);

	portguid = p_recvd_mcmember_rec->port_gid.unicast.interface_id;

	mcmember_rec = *p_recvd_mcmember_rec;

        /* Validate the subnet prefix in the PortGID */
	if (p_recvd_mcmember_rec->port_gid.unicast.prefix !=
	    sa->p_subn->opt.subnet_prefix) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"PortGID subnet prefix 0x%" PRIx64
			" does not match configured prefix 0x%" PRIx64 "\n",
			cl_ntoh64(p_recvd_mcmember_rec->port_gid.unicast.prefix),
			cl_ntoh64(sa->p_subn->opt.subnet_prefix));
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_INVALID_GID);
		goto Exit;
	}

	CL_PLOCK_EXCL_ACQUIRE(sa->p_lock);

	if (OSM_LOG_IS_ACTIVE_V2(sa->p_log, OSM_LOG_DEBUG)) {
		osm_physp_t *p_req_physp;

		p_req_physp = osm_get_physp_by_mad_addr(sa->p_log, sa->p_subn,
							osm_madw_get_mad_addr_ptr(p_madw));
		if (p_req_physp == NULL) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B03: "
				"Cannot find requester physical port\n");
		} else {
			OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
				"Requester port GUID 0x%" PRIx64 "\n",
				cl_ntoh64(osm_physp_get_port_guid(p_req_physp)));
		}
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG, "Dump of incoming record\n");
		osm_dump_mc_record_v2(sa->p_log, &mcmember_rec, FILE_ID, OSM_LOG_DEBUG);
	}

	/* make sure the requested port guid is known to the SM */
	p_port = osm_get_port_by_alias_guid(sa->p_subn, portguid);
	if (!p_port) {
		CL_PLOCK_RELEASE(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Unknown port GUID 0x%016" PRIx64 "\n",
			cl_ntoh64(portguid));
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_REQ_INVALID);
		goto Exit;
	}

	p_physp = p_port->p_physp;
	/* Check that the p_physp and the requester physp are in the same
	   partition. */
	p_request_physp =
	    osm_get_physp_by_mad_addr(sa->p_log, sa->p_subn,
				      osm_madw_get_mad_addr_ptr(p_madw));
	if (p_request_physp == NULL) {
		CL_PLOCK_RELEASE(sa->p_lock);
		goto Exit;
	}

	proxy = (p_physp != p_request_physp);

	if (proxy && !osm_physp_share_pkey(sa->p_log, p_physp, p_request_physp,
					   sa->p_subn->opt.allow_both_pkeys)) {
		CL_PLOCK_RELEASE(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_VERBOSE,
			"Port and requester don't share pkey\n");
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_REQ_INVALID);
		goto Exit;
	}

	if ((p_sa_mad->comp_mask & IB_MCR_COMPMASK_PKEY) &&
	    ib_pkey_is_invalid(p_recvd_mcmember_rec->pkey)) {
		CL_PLOCK_RELEASE(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_VERBOSE,
			"Invalid PKey supplied in request\n");
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_REQ_INVALID);
		goto Exit;
	}

	ib_member_get_scope_state(p_recvd_mcmember_rec->scope_state, NULL,
				  &join_state);

	/* do we need to create a new group? */
	p_mgrp = osm_get_mgrp_by_mgid(sa->p_subn, &p_recvd_mcmember_rec->mgid);
	if (!p_mgrp) {
		/* check for JoinState.FullMember = 1 o15.0.1.9 */
		if ((join_state & 0x01) != 0x01) {
			char gid_str[INET6_ADDRSTRLEN];
			CL_PLOCK_RELEASE(sa->p_lock);
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B10: "
				"Failed to create multicast group "
				"because Join State != FullMember, "
				"MGID: %s from port 0x%016" PRIx64 " (%s)\n",
				inet_ntop(AF_INET6,
					  p_recvd_mcmember_rec->mgid.raw,
					  gid_str, sizeof gid_str),
				cl_ntoh64(portguid),
				p_port->p_node->print_desc);
			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_REQ_INVALID);
			goto Exit;
		}

		/* check the comp_mask */
		if (!check_create_comp_mask(p_sa_mad->comp_mask,
					    p_recvd_mcmember_rec)) {
			char gid_str[INET6_ADDRSTRLEN];
			CL_PLOCK_RELEASE(sa->p_lock);
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B11: "
				"Port 0x%016" PRIx64 " (%s) failed to join "
				"non-existing multicast group with MGID %s, "
				"insufficient components specified for "
				"implicit create (comp_mask 0x%" PRIx64 ")\n",
				cl_ntoh64(portguid), p_port->p_node->print_desc,
				inet_ntop(AF_INET6,
					  p_recvd_mcmember_rec->mgid.raw,
					  gid_str, sizeof gid_str),
				cl_ntoh64(p_sa_mad->comp_mask));
			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_INSUF_COMPS);
			goto Exit;
		}

		status = mcmr_rcv_create_new_mgrp(sa, p_sa_mad->comp_mask,
						  p_recvd_mcmember_rec,
						  p_physp, &p_mgrp);
		if (status != IB_SUCCESS) {
			CL_PLOCK_RELEASE(sa->p_lock);
			osm_sa_send_error(sa, p_madw, status);
			goto Exit;
		}
		/* copy the MGID to the result */
		mcmember_rec.mgid = p_mgrp->mcmember_rec.mgid;
		is_new_group = 1;
	} else
		/* no need for a new group */
		is_new_group = 0;

	CL_ASSERT(p_mgrp);

	/*
	 * o15-0.2.4: If SA supports UD multicast, then SA shall cause an
	 * endport to join an existing multicast group if:
	 * 1. It receives a SubnAdmSet() method for a MCMemberRecord, and
	 *    - WE KNOW THAT ALREADY
	 * 2. The MGID is specified and matches an existing multicast
	 *    group, and
	 *    - WE KNOW THAT ALREADY
	 * 3. The MCMemberRecord:JoinState is not all 0s, and
	 * 4. PortGID is specified and
	 *    - WE KNOW THAT ALREADY (as it matched a real one)
	 * 5. All other components match that existing group, either by
	 *    being wildcarded or by having values identical to those specified
	 *    by the component mask and in use by the group with the exception
	 *    of components such as ProxyJoin and Reserved, which are ignored
	 *    by SA.
	 *
	 * We need to check #3 and #5 here:
	 */
	if (!validate_more_comp_fields(sa->p_log, p_mgrp, p_recvd_mcmember_rec,
				       p_sa_mad->comp_mask)
	    || !validate_port_caps(sa->p_log, p_mgrp, p_physp)
	    || !(join_state != 0)) {
		char gid_str[INET6_ADDRSTRLEN];
		/* since we might have created the new group we need to cleanup */
		if (is_new_group)
			osm_mgrp_cleanup(sa->p_subn, p_mgrp);
		CL_PLOCK_RELEASE(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B12: "
			"validate_more_comp_fields, validate_port_caps, "
			"or JoinState = 0 failed for MGID: %s port 0x%016" PRIx64
			" (%s), sending IB_SA_MAD_STATUS_REQ_INVALID\n",
			   inet_ntop(AF_INET6, p_mgrp->mcmember_rec.mgid.raw,
				     gid_str, sizeof gid_str),
			cl_ntoh64(portguid), p_port->p_node->print_desc);
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_REQ_INVALID);
		goto Exit;
	}

	/*
	 * o15-0.2.1 requires validation of the requesting port
	 * in the case of modification:
	 */
	if (!is_new_group &&
	    !validate_modify(sa, p_mgrp, osm_madw_get_mad_addr_ptr(p_madw),
			     p_recvd_mcmember_rec, &p_mcm_alias_guid)) {
		CL_PLOCK_RELEASE(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B13: "
			"validate_modify failed from port 0x%016" PRIx64
			" (%s), sending IB_SA_MAD_STATUS_REQ_INVALID\n",
			cl_ntoh64(portguid), p_port->p_node->print_desc);
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_REQ_INVALID);
		goto Exit;
	}

	/* copy qkey mlid tclass pkey sl_flow_hop mtu rate pkt_life sl_flow_hop */
	copy_from_create_mc_rec(&mcmember_rec, &p_mgrp->mcmember_rec);

	/* create or update existing port (join-state will be updated) */
	p_mcmr_port = osm_mgrp_add_port(sa->p_subn, sa->p_log, p_mgrp, p_port,
					&mcmember_rec, proxy);
	if (!p_mcmr_port) {
		/* we fail to add the port so we might need to delete the group */
		if (is_new_group)
			osm_mgrp_cleanup(sa->p_subn, p_mgrp);
		CL_PLOCK_RELEASE(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B06: "
			"osm_mgrp_add_port failed\n");
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_NO_RESOURCES);
		goto Exit;
	}

	/* Release the lock as we don't need it. */
	CL_PLOCK_RELEASE(sa->p_lock);

	if (OSM_LOG_IS_ACTIVE_V2(sa->p_log, OSM_LOG_DEBUG))
		osm_dump_mc_record_v2(sa->p_log, &mcmember_rec, FILE_ID, OSM_LOG_DEBUG);

	mcmr_rcv_respond(sa, p_madw, &mcmember_rec);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}