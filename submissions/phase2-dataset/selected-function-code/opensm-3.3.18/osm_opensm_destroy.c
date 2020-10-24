void osm_opensm_destroy(IN osm_opensm_t * p_osm)
{
	/* in case of shutdown through exit proc - no ^C */
	osm_exit_flag = TRUE;

	/*
	 * First of all, clear the is_sm bit.
	 */
	if (p_osm->sm.mad_ctrl.h_bind)
		osm_vendor_set_sm(p_osm->sm.mad_ctrl.h_bind, FALSE);

#ifdef ENABLE_OSM_PERF_MGR
	/* Shutdown the PerfMgr */
	osm_perfmgr_shutdown(&p_osm->perfmgr);
#endif				/* ENABLE_OSM_PERF_MGR */

	osm_congestion_control_shutdown(&p_osm->cc);

	/* shut down the SA
	 * - unbind from QP1 messages
	 */
	osm_sa_shutdown(&p_osm->sa);

	/* shut down the SM
	 * - make sure the SM sweeper thread exited
	 * - unbind from QP0 messages
	 */
	osm_sm_shutdown(&p_osm->sm);

	/* cleanup all messages on VL15 fifo that were not sent yet */
	osm_vl15_shutdown(&p_osm->vl15, &p_osm->mad_pool);

	/* shut down the dispatcher - so no new messages cross */
	cl_disp_shutdown(&p_osm->disp);

	/* dump SA DB */
	if ((p_osm->sm.p_subn->sm_state == IB_SMINFO_STATE_MASTER) &&
	     p_osm->subn.opt.sa_db_dump)
		osm_sa_db_file_dump(p_osm);

	/* do the destruction in reverse order as init */
	destroy_plugins(p_osm);
	destroy_routing_engines(p_osm);
	osm_sa_destroy(&p_osm->sa);
	osm_sm_destroy(&p_osm->sm);
#ifdef ENABLE_OSM_PERF_MGR
	osm_perfmgr_destroy(&p_osm->perfmgr);
#endif				/* ENABLE_OSM_PERF_MGR */
	osm_congestion_control_destroy(&p_osm->cc);
}