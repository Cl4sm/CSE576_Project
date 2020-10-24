static ib_net16_t get_new_mlid(osm_sa_t * sa, ib_member_rec_t * mcmr)
{
	osm_subn_t *p_subn = sa->p_subn;
	ib_net16_t requested_mlid = mcmr->mlid;
	unsigned i, max;

	if (requested_mlid && cl_ntoh16(requested_mlid) >= IB_LID_MCAST_START_HO
	    && cl_ntoh16(requested_mlid) <= p_subn->max_mcast_lid_ho
	    && !osm_get_mbox_by_mlid(p_subn, requested_mlid))
		return requested_mlid;

	if (sa->p_subn->opt.consolidate_ipv6_snm_req
	    && match_ipv6_snm_mgid(&mcmr->mgid)
	    && (requested_mlid = find_ipv6_snm_mlid(sa->p_subn, &mcmr->mgid))) {
		char str[INET6_ADDRSTRLEN];
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Special Case Solicited Node Mcast Join for MGID %s\n",
			inet_ntop(AF_INET6, mcmr->mgid.raw, str, sizeof(str)));
		return requested_mlid;
	}

	max = p_subn->max_mcast_lid_ho - IB_LID_MCAST_START_HO + 1;
	for (i = 0; i < max; i++)
		if (!sa->p_subn->mboxes[i])
			return cl_hton16(i + IB_LID_MCAST_START_HO);

	return 0;
}