void osm_sm_shutdown(IN osm_sm_t * p_sm)
{
	boolean_t signal_event = FALSE;

	OSM_LOG_ENTER(p_sm->p_log);

	/*
	 * Signal our threads that we're leaving.
	 */
	if (p_sm->thread_state != OSM_THREAD_STATE_NONE)
		signal_event = TRUE;

	p_sm->thread_state = OSM_THREAD_STATE_EXIT;

	/*
	 * Don't trigger unless event has been initialized.
	 * Destroy the thread before we tear down the other objects.
	 */
	if (signal_event)
		cl_event_signal(&p_sm->signal_event);

	cl_timer_stop(&p_sm->polling_timer);
	cl_timer_stop(&p_sm->sweep_timer);
	cl_thread_destroy(&p_sm->sweeper);

	/*
	 * Always destroy controllers before the corresponding
	 * receiver to guarantee that all callbacks from the
	 * dispatcher are complete.
	 */
	osm_sm_mad_ctrl_destroy(&p_sm->mad_ctrl);
	cl_disp_unregister(p_sm->ni_disp_h);
	cl_disp_unregister(p_sm->pi_disp_h);
	cl_disp_unregister(p_sm->gi_disp_h);
	cl_disp_unregister(p_sm->si_disp_h);
	cl_disp_unregister(p_sm->nd_disp_h);
	cl_disp_unregister(p_sm->lft_disp_h);
	cl_disp_unregister(p_sm->mft_disp_h);
	cl_disp_unregister(p_sm->sm_info_disp_h);
	cl_disp_unregister(p_sm->trap_disp_h);
	cl_disp_unregister(p_sm->slvl_disp_h);
	cl_disp_unregister(p_sm->vla_disp_h);
	cl_disp_unregister(p_sm->pkey_disp_h);
	cl_disp_unregister(p_sm->mlnx_epi_disp_h);
	cl_disp_unregister(p_sm->sweep_fail_disp_h);

	OSM_LOG_EXIT(p_sm->p_log);
}