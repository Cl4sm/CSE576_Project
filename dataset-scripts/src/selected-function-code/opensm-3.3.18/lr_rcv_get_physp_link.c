static void lr_rcv_get_physp_link(IN osm_sa_t * sa,
				  IN const ib_link_record_t * p_lr,
				  IN const osm_physp_t * p_src_physp,
				  IN const osm_physp_t * p_dest_physp,
				  IN const ib_net64_t comp_mask,
				  IN cl_qlist_t * p_list,
				  IN const osm_physp_t * p_req_physp)
{
	uint8_t src_port_num;
	uint8_t dest_port_num;
	ib_net16_t from_base_lid;
	ib_net16_t to_base_lid;
	ib_net16_t lmc_mask;

	OSM_LOG_ENTER(sa->p_log);

	/*
	   If only one end of the link is specified, determine
	   the other side.
	 */
	if (p_src_physp) {
		if (p_dest_physp) {
			/*
			   Ensure the two physp's are actually connected.
			   If not, bail out.
			 */
			if (osm_physp_get_remote(p_src_physp) != p_dest_physp)
				goto Exit;
		} else {
			p_dest_physp = osm_physp_get_remote(p_src_physp);
			if (p_dest_physp == NULL)
				goto Exit;
		}
	} else {
		if (p_dest_physp) {
			p_src_physp = osm_physp_get_remote(p_dest_physp);
			if (p_src_physp == NULL)
				goto Exit;
		} else
			goto Exit;	/* no physp's, so nothing to do */
	}

	/* Check that the p_src_physp, p_dest_physp and p_req_physp
	   all share a pkey (doesn't have to be the same p_key). */
	if (!osm_physp_share_pkey(sa->p_log, p_src_physp, p_dest_physp,
				  sa->p_subn->opt.allow_both_pkeys)) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Source and Dest PhysPorts do not share PKey\n");
		goto Exit;
	}
	if (!osm_physp_share_pkey(sa->p_log, p_src_physp, p_req_physp,
				  sa->p_subn->opt.allow_both_pkeys)) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Source and Requester PhysPorts do not share PKey\n");
		goto Exit;
	}
	if (!osm_physp_share_pkey(sa->p_log, p_req_physp, p_dest_physp,
				  sa->p_subn->opt.allow_both_pkeys)) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Requester and Dest PhysPorts do not share PKey\n");
		goto Exit;
	}

	src_port_num = osm_physp_get_port_num(p_src_physp);
	dest_port_num = osm_physp_get_port_num(p_dest_physp);

	if (comp_mask & IB_LR_COMPMASK_FROM_PORT)
		if (src_port_num != p_lr->from_port_num)
			goto Exit;

	if (comp_mask & IB_LR_COMPMASK_TO_PORT)
		if (dest_port_num != p_lr->to_port_num)
			goto Exit;

	from_base_lid = get_base_lid(p_src_physp);
	to_base_lid = get_base_lid(p_dest_physp);

	lmc_mask = ~((1 << sa->p_subn->opt.lmc) - 1);
	lmc_mask = cl_hton16(lmc_mask);

	if (comp_mask & IB_LR_COMPMASK_FROM_LID)
		if (from_base_lid != (p_lr->from_lid & lmc_mask))
			goto Exit;

	if (comp_mask & IB_LR_COMPMASK_TO_LID)
		if (to_base_lid != (p_lr->to_lid & lmc_mask))
			goto Exit;

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG, "Acquiring link record\n"
		"\t\t\t\tsrc port 0x%" PRIx64 " (port %u)"
		", dest port 0x%" PRIx64 " (port %u)\n",
		cl_ntoh64(osm_physp_get_port_guid(p_src_physp)), src_port_num,
		cl_ntoh64(osm_physp_get_port_guid(p_dest_physp)),
		dest_port_num);

	lr_rcv_build_physp_link(sa, from_base_lid, to_base_lid, src_port_num,
				dest_port_num, p_list);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}