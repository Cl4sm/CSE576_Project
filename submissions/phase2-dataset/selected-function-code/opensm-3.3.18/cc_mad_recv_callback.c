static void cc_mad_recv_callback(osm_madw_t * p_madw, void *bind_context,
				 osm_madw_t * p_req_madw)
{
	osm_congestion_control_t *p_cc = bind_context;

	OSM_LOG_ENTER(p_cc->log);

	CL_ASSERT(p_madw);

	/* HACK - should be extended when supporting CC traps */
	CL_ASSERT(p_req_madw != NULL);

	osm_madw_copy_context(p_madw, p_req_madw);
	osm_mad_pool_put(p_cc->mad_pool, p_req_madw);

	/* Do not decrement outstanding mads here, do it in the dispatcher */

	if (cl_disp_post(p_cc->cc_disp_h, OSM_MSG_MAD_CC,
			 p_madw, NULL, NULL) != CL_SUCCESS) {
		OSM_LOG(p_cc->log, OSM_LOG_ERROR, "ERR C105: "
			"Congestion Control Dispatcher post failed\n");
		osm_mad_pool_put(p_cc->mad_pool, p_madw);
	}

	OSM_LOG_EXIT(p_cc->log);
}