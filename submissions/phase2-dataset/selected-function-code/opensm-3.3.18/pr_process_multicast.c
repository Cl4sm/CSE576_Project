static void pr_process_multicast(osm_sa_t * sa, const ib_sa_mad_t *sa_mad,
				 cl_qlist_t *list)
{
	ib_path_rec_t *pr = ib_sa_mad_get_payload_ptr(sa_mad);
	osm_mgrp_t *mgrp;
	ib_api_status_t status;
	osm_sa_item_t *pr_item;
	uint32_t flow_label;
	uint8_t sl, hop_limit;

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG, "Multicast destination requested\n");

	mgrp = osm_get_mgrp_by_mgid(sa->p_subn, &pr->dgid);
	if (!mgrp) {
		char gid_str[INET6_ADDRSTRLEN];
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F09: "
			"No MC group found for PathRecord destination GID %s\n",
			inet_ntop(AF_INET6, pr->dgid.raw, gid_str,
				  sizeof gid_str));
		return;
	}

	/* Make sure the rest of the PathRecord matches the MC group attributes */
	status = pr_match_mgrp_attributes(sa, sa_mad, mgrp);
	if (status != IB_SUCCESS) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F19: "
			"MC group attributes don't match PathRecord request\n");
		return;
	}

	pr_item = malloc(SA_PR_RESP_SIZE);
	if (pr_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F18: "
			"Unable to allocate path record for MC group\n");
		return;
	}
	memset(pr_item, 0, sizeof(cl_list_item_t));

	/* Copy PathRecord request into response */
	pr_item->resp.path_rec = *pr;

	/* Now, use the MC info to cruft up the PathRecord response */
	pr_item->resp.path_rec.dgid = mgrp->mcmember_rec.mgid;
	pr_item->resp.path_rec.dlid = mgrp->mcmember_rec.mlid;
	pr_item->resp.path_rec.tclass = mgrp->mcmember_rec.tclass;
	pr_item->resp.path_rec.num_path = 1;
	pr_item->resp.path_rec.pkey = mgrp->mcmember_rec.pkey;

	/* MTU, rate, and packet lifetime should be exactly */
	pr_item->resp.path_rec.mtu = (IB_PATH_SELECTOR_EXACTLY << 6) | mgrp->mcmember_rec.mtu;
	pr_item->resp.path_rec.rate = (IB_PATH_SELECTOR_EXACTLY << 6) | mgrp->mcmember_rec.rate;
	pr_item->resp.path_rec.pkt_life = (IB_PATH_SELECTOR_EXACTLY << 6) | mgrp->mcmember_rec.pkt_life;

	/* SL, Hop Limit, and Flow Label */
	ib_member_get_sl_flow_hop(mgrp->mcmember_rec.sl_flow_hop,
				  &sl, &flow_label, &hop_limit);
	ib_path_rec_set_sl(&pr_item->resp.path_rec, sl);
	ib_path_rec_set_qos_class(&pr_item->resp.path_rec, 0);

	/* HopLimit is not yet set in non link local MC groups */
	/* If it were, this would not be needed */
	if (ib_mgid_get_scope(&mgrp->mcmember_rec.mgid) !=
	    IB_MC_SCOPE_LINK_LOCAL)
		hop_limit = IB_HOPLIMIT_MAX;

	pr_item->resp.path_rec.hop_flow_raw =
	    cl_hton32(hop_limit) | (flow_label << 8);

	cl_qlist_insert_tail(list, &pr_item->list_item);
}