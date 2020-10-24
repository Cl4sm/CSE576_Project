static void sm_mad_ctrl_disp_done_callback(IN void *context, IN void *p_data)
{
	osm_sm_mad_ctrl_t *p_ctrl = context;
	osm_madw_t *p_madw = p_data;
	ib_smp_t *p_smp;

	OSM_LOG_ENTER(p_ctrl->p_log);

	/*
	   If the MAD that just finished processing was a response,
	   then retire the transaction, since we must have generated
	   the request.

	   Otherwise, retire the transaction if a response was expected,
	   as in the case of a send failure. If a response was not expected,
	   just put the MAD back in the pool, because the MAD was a query
	   from some outside agent, e.g. Get(SMInfo) from another SM.
	 */
	p_smp = osm_madw_get_smp_ptr(p_madw);
	if (ib_smp_is_response(p_smp)) {
		CL_ASSERT(p_madw->resp_expected == FALSE);
		sm_mad_ctrl_retire_trans_mad(p_ctrl, p_madw);
	} else if (p_madw->resp_expected == TRUE)
		sm_mad_ctrl_retire_trans_mad(p_ctrl, p_madw);
	else
		osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);

	OSM_LOG_EXIT(p_ctrl->p_log);
}