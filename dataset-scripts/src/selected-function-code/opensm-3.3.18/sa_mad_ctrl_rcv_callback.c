static void sa_mad_ctrl_rcv_callback(IN osm_madw_t * p_madw, IN void *context,
				     IN osm_madw_t * p_req_madw)
{
	osm_sa_mad_ctrl_t *p_ctrl = context;
	ib_sa_mad_t *p_sa_mad;

	OSM_LOG_ENTER(p_ctrl->p_log);

	CL_ASSERT(p_madw);

	/*
	   A MAD was received from the wire, possibly in response to a request.
	 */
	cl_atomic_inc(&p_ctrl->p_stats->sa_mads_rcvd);

	OSM_LOG(p_ctrl->p_log, OSM_LOG_DEBUG,
		"%u SA MADs received\n", p_ctrl->p_stats->sa_mads_rcvd);

	/*
	 * C15-0.1.3 requires not responding to any MAD if the SM is
	 * not in active state!
	 * We will not respond if the sm_state is not MASTER, or if the
	 * first_time_master_sweep flag (of the subnet) is TRUE - this
	 * flag indicates that the master still didn't finish its first
	 * sweep, so the subnet is not up and stable yet.
	 */
	if (p_ctrl->p_subn->sm_state != IB_SMINFO_STATE_MASTER) {
		cl_atomic_inc(&p_ctrl->p_stats->sa_mads_ignored);
		OSM_LOG(p_ctrl->p_log, OSM_LOG_VERBOSE,
			"Received SA MAD while SM not MASTER. MAD ignored\n");
		osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);
		goto Exit;
	}
	if (p_ctrl->p_subn->first_time_master_sweep == TRUE) {
		cl_atomic_inc(&p_ctrl->p_stats->sa_mads_ignored);
		OSM_LOG(p_ctrl->p_log, OSM_LOG_VERBOSE,
			"Received SA MAD while SM in first sweep. MAD ignored\n");
		osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);
		goto Exit;
	}

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);

	if (OSM_LOG_IS_ACTIVE_V2(p_ctrl->p_log, OSM_LOG_FRAMES))
		osm_dump_sa_mad_v2(p_ctrl->p_log, p_sa_mad, FILE_ID, OSM_LOG_FRAMES);

	/*
	 * C15-0.1.5 - Table 185: SA Header - p884
	 * SM_key should be either 0 or match the current SM_Key
	 * otherwise discard the MAD.
	 */
	if (p_sa_mad->sm_key != 0 &&
	    p_sa_mad->sm_key != p_ctrl->p_subn->opt.sa_key) {
		OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 1A04: "
			"Non-Zero MAD SM_Key: 0x%" PRIx64 " != SM_Key: 0x%"
			PRIx64 "; SA MAD ignored for method 0x%X attribute 0x%X (%s)\n",
			cl_ntoh64(p_sa_mad->sm_key),
			cl_ntoh64(p_ctrl->p_subn->opt.sa_key),
			p_sa_mad->method, cl_ntoh16(p_sa_mad->attr_id),
			ib_get_sa_attr_str(p_sa_mad->attr_id));
		osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);
		goto Exit;
	}

	switch (p_sa_mad->method) {
	case IB_MAD_METHOD_REPORT_RESP:
		/* we do not really do anything with report represses -
		   just retire the transaction */
		OSM_LOG(p_ctrl->p_log, OSM_LOG_DEBUG,
			"Received Report Repress. Retiring the transaction\n");

		if (p_req_madw)
			osm_mad_pool_put(p_ctrl->p_mad_pool, p_req_madw);
		osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);

		break;

	case IB_MAD_METHOD_GET:
	case IB_MAD_METHOD_GETTABLE:
#if defined (VENDOR_RMPP_SUPPORT) && defined (DUAL_SIDED_RMPP)
	case IB_MAD_METHOD_GETMULTI:
#endif
	case IB_MAD_METHOD_SET:
	case IB_MAD_METHOD_DELETE:
		/* if we are closing down simply do nothing */
		if (osm_exit_flag)
			osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);
		else
			sa_mad_ctrl_process(p_ctrl, p_madw);
		break;

	default:
		cl_atomic_inc(&p_ctrl->p_stats->sa_mads_rcvd_unknown);
		OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 1A05: "
			"Unsupported method = 0x%X\n", p_sa_mad->method);
		osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(p_ctrl->p_log);
}