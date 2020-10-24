void osm_sa_shutdown(IN osm_sa_t * p_sa)
{
	OSM_LOG_ENTER(p_sa->p_log);

	cl_timer_stop(&p_sa->sr_timer);

	/* unbind from the mad service */
	osm_sa_mad_ctrl_unbind(&p_sa->mad_ctrl);

	/* remove any registered dispatcher message */
	cl_disp_unregister(p_sa->nr_disp_h);
	cl_disp_unregister(p_sa->pir_disp_h);
	cl_disp_unregister(p_sa->gir_disp_h);
	cl_disp_unregister(p_sa->lr_disp_h);
	cl_disp_unregister(p_sa->pr_disp_h);
#if defined (VENDOR_RMPP_SUPPORT) && defined (DUAL_SIDED_RMPP)
	cl_disp_unregister(p_sa->mpr_disp_h);
#endif
	cl_disp_unregister(p_sa->smir_disp_h);
	cl_disp_unregister(p_sa->mcmr_disp_h);
	cl_disp_unregister(p_sa->sr_disp_h);
	cl_disp_unregister(p_sa->infr_disp_h);
	cl_disp_unregister(p_sa->infir_disp_h);
	cl_disp_unregister(p_sa->vlarb_disp_h);
	cl_disp_unregister(p_sa->slvl_disp_h);
	cl_disp_unregister(p_sa->pkey_disp_h);
	cl_disp_unregister(p_sa->lft_disp_h);
	cl_disp_unregister(p_sa->sir_disp_h);
	cl_disp_unregister(p_sa->mft_disp_h);
	osm_sa_mad_ctrl_destroy(&p_sa->mad_ctrl);

	OSM_LOG_EXIT(p_sa->p_log);
}