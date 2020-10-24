static void sm_mad_ctrl_send_err_cb(IN void *context, IN osm_madw_t * p_madw)
{
	osm_sm_mad_ctrl_t *p_ctrl = context;
	ib_api_status_t status;
	ib_smp_t *p_smp;

	OSM_LOG_ENTER(p_ctrl->p_log);

	CL_ASSERT(p_madw);

	p_smp = osm_madw_get_smp_ptr(p_madw);
	OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 3113: "
		"MAD completed in error (%s): "
		"%s(%s), attr_mod 0x%x, TID 0x%" PRIx64 "\n",
		ib_get_err_str(p_madw->status),
		ib_get_sm_method_str(p_smp->method),
		ib_get_sm_attr_str(p_smp->attr_id), cl_ntoh32(p_smp->attr_mod),
		cl_ntoh64(p_smp->trans_id));

	/*
	   If this was a SubnSet MAD, then this error might indicate a problem
	   in configuring the subnet. In this case - need to mark that there was
	   such a problem. The subnet will not be up, and the next sweep should
	   be a heavy sweep as well.
	 */
	if (p_smp->method == IB_MAD_METHOD_SET &&
	    (p_smp->attr_id == IB_MAD_ATTR_PORT_INFO ||
	     p_smp->attr_id == IB_MAD_ATTR_MLNX_EXTENDED_PORT_INFO ||
	     p_smp->attr_id == IB_MAD_ATTR_MCAST_FWD_TBL ||
	     p_smp->attr_id == IB_MAD_ATTR_SWITCH_INFO ||
	     p_smp->attr_id == IB_MAD_ATTR_LIN_FWD_TBL ||
	     p_smp->attr_id == IB_MAD_ATTR_P_KEY_TABLE)) {
		OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 3119: "
			"Set method failed for attribute 0x%X (%s)\n",
			cl_ntoh16(p_smp->attr_id),
			ib_get_sm_attr_str(p_smp->attr_id));
		p_ctrl->p_subn->subnet_initialization_error = TRUE;
	} else if (p_madw->status == IB_TIMEOUT &&
		   p_smp->method == IB_MAD_METHOD_GET) {
		OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 3120 "
			"Timeout while getting attribute 0x%X (%s); "
			"Possible mis-set mkey?\n",
			cl_ntoh16(p_smp->attr_id),
			ib_get_sm_attr_str(p_smp->attr_id));
	}

	osm_dump_dr_smp_v2(p_ctrl->p_log, p_smp, FILE_ID, OSM_LOG_VERBOSE);

	/*
	   Since we did not get any response we suspect the DR path
	   used for the target port.
	   Find it and replace it with an alternate path.
	   This is true only if the destination lid is not 0xFFFF, since
	   then we are aiming for a specific path and not specific destination
	   lid.
	 */
	/* For now - do not add the alternate dr path to the release */
#if 0
	if (p_madw->mad_addr.dest_lid != 0xFFFF) {
		osm_physp_t *p_physp = osm_get_physp_by_mad_addr(p_ctrl->p_log,
								 p_ctrl->p_subn,
								 &(p_madw->
								   mad_addr));
		if (!p_physp) {
			OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 3114: "
				"Failed to find the corresponding phys port\n");
		} else {
			osm_physp_replace_dr_path_with_alternate_dr_path
			    (p_ctrl->p_log, p_ctrl->p_subn, p_physp,
			     p_madw->h_bind);
		}
	}
#endif

	/*
	   An error occurred.  No response was received to a request MAD.
	   Retire the original request MAD.
	 */
	sm_mad_ctrl_update_wire_stats(p_ctrl);

	if (osm_madw_get_err_msg(p_madw) != CL_DISP_MSGID_NONE) {
		OSM_LOG(p_ctrl->p_log, OSM_LOG_DEBUG,
			"Posting Dispatcher message %s\n",
			osm_get_disp_msg_str(osm_madw_get_err_msg(p_madw)));

		status = cl_disp_post(p_ctrl->h_disp,
				      osm_madw_get_err_msg(p_madw), p_madw,
				      sm_mad_ctrl_disp_done_callback, p_ctrl);
		if (status != CL_SUCCESS)
			OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 3115: "
				"Dispatcher post message failed (%s)\n",
				CL_STATUS_MSG(status));
	} else
		/*
		   No error message was provided, just retire the MAD.
		 */
		sm_mad_ctrl_retire_trans_mad(p_ctrl, p_madw);

	OSM_LOG_EXIT(p_ctrl->p_log);
}