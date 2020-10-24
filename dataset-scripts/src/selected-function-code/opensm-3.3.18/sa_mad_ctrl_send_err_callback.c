static void sa_mad_ctrl_send_err_callback(IN void *context,
					  IN osm_madw_t * p_madw)
{
	osm_sa_mad_ctrl_t *p_ctrl = context;
	cl_status_t status;

	OSM_LOG_ENTER(p_ctrl->p_log);

	/*
	   We should never be here since the SA never originates a request.
	   Unless we generated a Report(Notice)
	 */

	CL_ASSERT(p_madw);

	OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 1A06: "
		"MAD completed in error (%s): "
		"%s(%s), attr_mod 0x%x, LID %u, TID 0x%" PRIx64 "\n",
		ib_get_err_str(p_madw->status),
		ib_get_sa_method_str(p_madw->p_mad->method),
		ib_get_sa_attr_str(p_madw->p_mad->attr_id),
		cl_ntoh32(p_madw->p_mad->attr_mod),
		cl_ntoh16(p_madw->mad_addr.dest_lid),
		cl_ntoh64(p_madw->p_mad->trans_id));

	osm_dump_sa_mad_v2(p_ctrl->p_log, osm_madw_get_sa_mad_ptr(p_madw),
			   FILE_ID, OSM_LOG_ERROR);

	/*
	   An error occurred.  No response was received to a request MAD.
	   Retire the original request MAD.
	 */

	if (osm_madw_get_err_msg(p_madw) != CL_DISP_MSGID_NONE) {
		OSM_LOG(p_ctrl->p_log, OSM_LOG_DEBUG,
			"Posting Dispatcher message %s\n",
			osm_get_disp_msg_str(osm_madw_get_err_msg(p_madw)));

		status = cl_disp_post(p_ctrl->h_disp,
				      osm_madw_get_err_msg(p_madw), p_madw,
				      sa_mad_ctrl_disp_done_callback, p_ctrl);
		if (status != CL_SUCCESS) {
			OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 1A07: "
				"Dispatcher post message failed (%s)\n",
				CL_STATUS_MSG(status));
		}
	} else			/* No error message was provided, just retire the MAD. */
		osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);

	OSM_LOG_EXIT(p_ctrl->p_log);
}