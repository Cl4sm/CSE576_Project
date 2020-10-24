static void mcmr_rcv_leave_mgrp(IN osm_sa_t * sa, IN osm_madw_t * p_madw)
{
	osm_mgrp_t *p_mgrp;
	ib_sa_mad_t *p_sa_mad;
	ib_member_rec_t *p_recvd_mcmember_rec;
	ib_member_rec_t mcmember_rec;
	osm_mcm_alias_guid_t *p_mcm_alias_guid;

	OSM_LOG_ENTER(sa->p_log);

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_recvd_mcmember_rec =
	    (ib_member_rec_t *) ib_sa_mad_get_payload_ptr(p_sa_mad);

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
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B02: "
				"Cannot find requester physical port\n");
		} else {
			OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
				"Requester port GUID 0x%" PRIx64 "\n",
				cl_ntoh64(osm_physp_get_port_guid(p_req_physp)));
		}
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG, "Dump of record\n");
		osm_dump_mc_record_v2(sa->p_log, &mcmember_rec, FILE_ID, OSM_LOG_DEBUG);
	}

	p_mgrp = osm_get_mgrp_by_mgid(sa->p_subn, &p_recvd_mcmember_rec->mgid);
	if (!p_mgrp) {
		char gid_str[INET6_ADDRSTRLEN];
		CL_PLOCK_RELEASE(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Failed since multicast group %s not present\n",
			inet_ntop(AF_INET6, p_recvd_mcmember_rec->mgid.raw,
				  gid_str, sizeof gid_str));
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_REQ_INVALID);
		goto Exit;
	}

	/* check validity of the delete request o15-0.1.14 */
	if (!validate_delete(sa, p_mgrp, osm_madw_get_mad_addr_ptr(p_madw),
			     p_recvd_mcmember_rec, &p_mcm_alias_guid)) {
		char gid_str[INET6_ADDRSTRLEN];
		char gid_str2[INET6_ADDRSTRLEN];
		CL_PLOCK_RELEASE(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B25: "
			"Received an invalid delete request for "
			"MGID: %s for PortGID: %s\n",
			inet_ntop(AF_INET6, p_recvd_mcmember_rec->mgid.raw,
				  gid_str, sizeof gid_str),
			inet_ntop(AF_INET6, p_recvd_mcmember_rec->port_gid.raw,
				  gid_str2, sizeof gid_str2));
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_REQ_INVALID);
		goto Exit;
	}

	/* remove port and/or update join state */
	osm_mgrp_remove_port(sa->p_subn, sa->p_log, p_mgrp, p_mcm_alias_guid,
			     &mcmember_rec);
	CL_PLOCK_RELEASE(sa->p_lock);

	mcmr_rcv_respond(sa, p_madw, &mcmember_rec);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}