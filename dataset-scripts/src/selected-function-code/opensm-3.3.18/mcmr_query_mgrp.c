static void mcmr_query_mgrp(IN osm_sa_t * sa, IN osm_madw_t * p_madw)
{
	const ib_sa_mad_t *p_rcvd_mad;
	const ib_member_rec_t *p_rcvd_rec;
	cl_qlist_t rec_list;
	ib_net64_t comp_mask;
	osm_physp_t *p_req_physp;
	boolean_t trusted_req;
	osm_mgrp_t *p_mgrp;

	OSM_LOG_ENTER(sa->p_log);

	p_rcvd_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_rcvd_rec = (ib_member_rec_t *) ib_sa_mad_get_payload_ptr(p_rcvd_mad);
	comp_mask = p_rcvd_mad->comp_mask;

	/*
	   if sm_key is not zero and does not match we never get here
	   see main SA receiver
	 */
	trusted_req = (p_rcvd_mad->sm_key != 0);

	CL_PLOCK_ACQUIRE(sa->p_lock);

	/* update the requester physical port */
	p_req_physp = osm_get_physp_by_mad_addr(sa->p_log, sa->p_subn,
						osm_madw_get_mad_addr_ptr
						(p_madw));
	if (p_req_physp == NULL) {
		CL_PLOCK_RELEASE(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1B04: "
			"Cannot find requester physical port\n");
		goto Exit;
	}

	if (OSM_LOG_IS_ACTIVE_V2(sa->p_log, OSM_LOG_DEBUG)) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Requester port GUID 0x%" PRIx64 "\n",
			cl_ntoh64(osm_physp_get_port_guid(p_req_physp)));
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG, "Dump of record\n");
		osm_dump_mc_record(sa->p_log, p_rcvd_rec, OSM_LOG_DEBUG);
	}

	cl_qlist_init(&rec_list);

	/* simply go over all MCGs and match */
	for (p_mgrp = (osm_mgrp_t *) cl_fmap_head(&sa->p_subn->mgrp_mgid_tbl);
	     p_mgrp != (osm_mgrp_t *) cl_fmap_end(&sa->p_subn->mgrp_mgid_tbl);
	     p_mgrp = (osm_mgrp_t *) cl_fmap_next(&p_mgrp->map_item))
		mcmr_by_comp_mask(sa, p_rcvd_rec, comp_mask, p_mgrp,
				  p_req_physp, trusted_req, &rec_list);

	CL_PLOCK_RELEASE(sa->p_lock);

	/*
	   p923 - The PortGID, JoinState and ProxyJoin shall be zero,
	   except in the case of a trusted request.
	   Note: In the mad controller we check that the SM_Key received on
	   the mad is valid. Meaning - is either zero or equal to the local
	   sm_key.
	 */

	if (!p_rcvd_mad->sm_key) {
		osm_sa_item_t *item;
		for (item = (osm_sa_item_t *) cl_qlist_head(&rec_list);
		     item != (osm_sa_item_t *) cl_qlist_end(&rec_list);
		     item =
		     (osm_sa_item_t *) cl_qlist_next(&item->list_item)) {
			memset(&item->resp.mc_rec.port_gid, 0, sizeof(ib_gid_t));
			ib_member_set_join_state(&item->resp.mc_rec, 0);
			item->resp.mc_rec.proxy_join = 0;
		}
	}

	osm_sa_respond(sa, p_madw, sizeof(ib_member_rec_t), &rec_list);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}