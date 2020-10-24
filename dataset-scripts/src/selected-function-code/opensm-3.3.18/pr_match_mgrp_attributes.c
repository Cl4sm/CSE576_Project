static ib_api_status_t pr_match_mgrp_attributes(IN osm_sa_t * sa,
						IN const ib_sa_mad_t * sa_mad,
						IN const osm_mgrp_t * p_mgrp)
{
	const ib_path_rec_t *p_pr = ib_sa_mad_get_payload_ptr(sa_mad);
	ib_net64_t comp_mask = sa_mad->comp_mask;
	const osm_port_t *port;
	ib_api_status_t status = IB_ERROR;
	uint32_t flow_label;
	uint8_t sl, hop_limit;

	OSM_LOG_ENTER(sa->p_log);

	/* check that MLID of the MC group matches the PathRecord DLID */
	if ((comp_mask & IB_PR_COMPMASK_DLID) && p_mgrp->mlid != p_pr->dlid) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"DLID 0x%x is not MLID 0x%x for MC group\n",
			 cl_ntoh16(p_pr->dlid), cl_ntoh16(p_mgrp->mlid));
		goto Exit;
	}

	/* If SGID and/or SLID specified, should validate as member of MC group */
	if (comp_mask & IB_PR_COMPMASK_SGID) {
		if (!osm_mgrp_get_mcm_alias_guid(p_mgrp,
						 p_pr->sgid.unicast.interface_id)) {
			char gid_str[INET6_ADDRSTRLEN];
			OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
				"SGID %s is not a member of MC group\n",
				inet_ntop(AF_INET6, p_pr->sgid.raw,
					  gid_str, sizeof gid_str));
			goto Exit;
		}
	}

	if (comp_mask & IB_PR_COMPMASK_SLID) {
		port = osm_get_port_by_lid(sa->p_subn, p_pr->slid);
		if (!port || !osm_mgrp_get_mcm_port(p_mgrp, port->guid)) {
			OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
				"Either no port with SLID %u found or "
				"SLID not a member of MC group\n",
				cl_ntoh16(p_pr->slid));
			goto Exit;
		}
	}

	/* Also, MTU, rate, packet lifetime, and raw traffic requested are not currently checked */
	if ((comp_mask & IB_PR_COMPMASK_PKEY) &&
	    p_pr->pkey != p_mgrp->mcmember_rec.pkey) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Pkey 0x%x doesn't match MC group Pkey 0x%x\n",
			cl_ntoh16(p_pr->pkey),
			cl_ntoh16(p_mgrp->mcmember_rec.pkey));
		goto Exit;
	}

	ib_member_get_sl_flow_hop(p_mgrp->mcmember_rec.sl_flow_hop,
				  &sl, &flow_label, &hop_limit);

	if ((comp_mask & IB_PR_COMPMASK_SL) && ib_path_rec_sl(p_pr) != sl) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"SL %d doesn't match MC group SL %d\n",
			ib_path_rec_sl(p_pr), sl);
		goto Exit;
	}

	/* If SubnAdmGet, assume NumbPaths of 1 (1.2 erratum) */
	if ((comp_mask & IB_PR_COMPMASK_NUMBPATH) &&
	    sa_mad->method != IB_MAD_METHOD_GET &&
	    ib_path_rec_num_path(p_pr) == 0) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Number of paths requested is 0\n");
		goto Exit;
	}

	if ((comp_mask & IB_PR_COMPMASK_FLOWLABEL) &&
	    ib_path_rec_flow_lbl(p_pr) != flow_label) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Flow label 0x%x doesn't match MC group "
			" flow label 0x%x\n",
			ib_path_rec_flow_lbl(p_pr), flow_label);
		goto Exit;
	}

	if ((comp_mask & IB_PR_COMPMASK_HOPLIMIT) &&
	    ib_path_rec_hop_limit(p_pr) != hop_limit) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Hop limit %u doesn't match MC group hop limit %u\n",
			ib_path_rec_hop_limit(p_pr), hop_limit);
		goto Exit;
	}


	if ((comp_mask & IB_PR_COMPMASK_TCLASS) &&
	    p_pr->tclass != p_mgrp->mcmember_rec.tclass) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"TClass 0x%02x doesn't match MC group TClass 0x%02x\n",
			p_pr->tclass, p_mgrp->mcmember_rec.tclass);
		goto Exit;
	}

	status = IB_SUCCESS;

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return status;
}