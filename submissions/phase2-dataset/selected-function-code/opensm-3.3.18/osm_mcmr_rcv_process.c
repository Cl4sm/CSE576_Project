void osm_mcmr_rcv_process(IN void *context, IN void *data)
{
	osm_sa_t *sa = context;
	osm_madw_t *p_madw = data;
	ib_sa_mad_t *p_sa_mad;
	ib_member_rec_t *p_recvd_mcmember_rec;

	CL_ASSERT(sa);

	OSM_LOG_ENTER(sa->p_log);

	CL_ASSERT(p_madw);

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_recvd_mcmember_rec =
	    (ib_member_rec_t *) ib_sa_mad_get_payload_ptr(p_sa_mad);

	CL_ASSERT(p_sa_mad->attr_id == IB_MAD_ATTR_MCMEMBER_RECORD);

	switch (p_sa_mad->method) {
	case IB_MAD_METHOD_SET:
		if (!check_join_comp_mask(p_sa_mad->comp_mask)) {
			char gid_str[INET6_ADDRSTRLEN];
			char gid_str2[INET6_ADDRSTRLEN];
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B18: "
				"component mask = 0x%016" PRIx64 ", "
				"expected comp mask = 0x%016" PRIx64 ", "
				"MGID: %s for PortGID: %s\n",
				cl_ntoh64(p_sa_mad->comp_mask),
				CL_NTOH64(JOIN_MC_COMP_MASK),
				inet_ntop(AF_INET6,
					  p_recvd_mcmember_rec->mgid.raw,
					  gid_str, sizeof gid_str),
				inet_ntop(AF_INET6,
					  p_recvd_mcmember_rec->port_gid.raw,
					  gid_str2, sizeof gid_str2));
			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_INSUF_COMPS);
			goto Exit;
		}
		if (!rate_is_valid(p_sa_mad, p_recvd_mcmember_rec) ||
		    !mtu_is_valid(p_sa_mad, p_recvd_mcmember_rec)) {
			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_REQ_INVALID);
			goto Exit;
		}

		/*
		 * Join or Create Multicast Group
		 */
		mcmr_rcv_join_mgrp(sa, p_madw);
		break;
	case IB_MAD_METHOD_DELETE:
		if (!check_join_comp_mask(p_sa_mad->comp_mask)) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B20: "
				"component mask = 0x%016" PRIx64 ", "
				"expected comp mask = 0x%016" PRIx64 "\n",
				cl_ntoh64(p_sa_mad->comp_mask),
				CL_NTOH64(JOIN_MC_COMP_MASK));
			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_INSUF_COMPS);
			goto Exit;
		}
		if (!rate_is_valid(p_sa_mad, p_recvd_mcmember_rec) ||
		    !mtu_is_valid(p_sa_mad, p_recvd_mcmember_rec)) {
			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_REQ_INVALID);
			goto Exit;
		}

		/*
		 * Leave Multicast Group
		 */
		mcmr_rcv_leave_mgrp(sa, p_madw);
		break;
	case IB_MAD_METHOD_GET:
	case IB_MAD_METHOD_GETTABLE:
		if (!rate_is_valid(p_sa_mad, p_recvd_mcmember_rec) ||
		    !mtu_is_valid(p_sa_mad, p_recvd_mcmember_rec)) {
			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_REQ_INVALID);
			goto Exit;
		}

		/*
		 * Querying a Multicast Group
		 */
		mcmr_query_mgrp(sa, p_madw);
		break;
	default:
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B21: "
			"Unsupported Method (%s) for MCMemberRecord request\n",
			ib_get_sa_method_str(p_sa_mad->method));
		osm_sa_send_error(sa, p_madw, IB_MAD_STATUS_UNSUP_METHOD_ATTR);
		break;
	}

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return;
}