static void sm_mad_ctrl_process_trap_repress(IN osm_sm_mad_ctrl_t * p_ctrl,
					     IN osm_madw_t * p_madw)
{
	ib_smp_t *p_smp;

	OSM_LOG_ENTER(p_ctrl->p_log);

	p_smp = osm_madw_get_smp_ptr(p_madw);

	/*
	   Note that attr_id (like the rest of the MAD) is in
	   network byte order.
	 */
	switch (p_smp->attr_id) {
	case IB_MAD_ATTR_NOTICE:
		sm_mad_ctrl_update_wire_stats(p_ctrl);
		sm_mad_ctrl_retire_trans_mad(p_ctrl, p_madw);
		break;
	default:
		cl_atomic_inc(&p_ctrl->p_stats->qp0_mads_rcvd_unknown);
		OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 3105: "
			"Unsupported attribute 0x%X (%s)\n",
			cl_ntoh16(p_smp->attr_id),
			ib_get_sm_attr_str(p_smp->attr_id));
		osm_dump_dr_smp_v2(p_ctrl->p_log, p_smp, FILE_ID, OSM_LOG_ERROR);
		osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);
		break;
	}

	OSM_LOG_EXIT(p_ctrl->p_log);
}