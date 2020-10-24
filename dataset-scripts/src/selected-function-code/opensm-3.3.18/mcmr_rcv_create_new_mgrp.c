static ib_api_status_t mcmr_rcv_create_new_mgrp(IN osm_sa_t * sa,
						IN ib_net64_t comp_mask,
						IN const ib_member_rec_t *
						p_recvd_mcmember_rec,
						IN const osm_physp_t * p_physp,
						OUT osm_mgrp_t ** pp_mgrp)
{
	ib_net16_t mlid;
	ib_api_status_t status = IB_SUCCESS;
	ib_member_rec_t mcm_rec = *p_recvd_mcmember_rec;	/* copy for modifications */

	OSM_LOG_ENTER(sa->p_log);

	/* we need to create the new MGID if it was not defined */
	if (!ib_gid_is_notzero(&p_recvd_mcmember_rec->mgid)) {
		/* create a new MGID */
		char gid_str[INET6_ADDRSTRLEN];

		if (!build_new_mgid(sa, comp_mask, &mcm_rec)) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B23: "
				"cannot allocate unique MGID value\n");
			status = IB_SA_MAD_STATUS_NO_RESOURCES;
			goto Exit;
		}
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG, "Allocated new MGID:%s\n",
			inet_ntop(AF_INET6, mcm_rec.mgid.raw, gid_str,
				  sizeof gid_str));
	} else if (!validate_requested_mgid(sa, &mcm_rec)) {
		/* a specific MGID was requested so validate the resulting MGID */
		status = IB_SA_MAD_STATUS_REQ_INVALID;
		goto Exit;
	}

	/* check the requested parameters are realizable */
	if (mgrp_request_is_realizable(sa, comp_mask, &mcm_rec, p_physp) ==
	    FALSE) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B26: "
			"Requested MGRP parameters are not realizable\n");
		status = IB_SA_MAD_STATUS_REQ_INVALID;
		goto Exit;
	}

	mlid = get_new_mlid(sa, &mcm_rec);
	if (mlid == 0) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B19: "
			"get_new_mlid failed request mlid 0x%04x\n",
			cl_ntoh16(mcm_rec.mlid));
		status = IB_SA_MAD_STATUS_NO_RESOURCES;
		goto Exit;
	}

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG, "Obtained new mlid 0x%X\n",
		cl_ntoh16(mlid));

	mcm_rec.mlid = mlid;
	/* create a new MC Group */
	*pp_mgrp = osm_mgrp_new(sa->p_subn, mlid, &mcm_rec);
	if (*pp_mgrp == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B08: "
			"osm_mgrp_new failed\n");
		free_mlid(sa, mlid);
		status = IB_SA_MAD_STATUS_NO_RESOURCES;
		goto Exit;
	}

	/* the mcmember_record should have mtu_sel, rate_sel, and pkt_lifetime_sel = 2 */
	(*pp_mgrp)->mcmember_rec.mtu &= 0x3f;
	(*pp_mgrp)->mcmember_rec.mtu |= IB_PATH_SELECTOR_EXACTLY << 6;
	(*pp_mgrp)->mcmember_rec.rate &= 0x3f;
	(*pp_mgrp)->mcmember_rec.rate |= IB_PATH_SELECTOR_EXACTLY << 6;
	(*pp_mgrp)->mcmember_rec.pkt_life &= 0x3f;
	(*pp_mgrp)->mcmember_rec.pkt_life |= IB_PATH_SELECTOR_EXACTLY << 6;

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return status;
}