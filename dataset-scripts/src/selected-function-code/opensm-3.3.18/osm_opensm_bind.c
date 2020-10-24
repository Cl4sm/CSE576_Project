ib_api_status_t osm_opensm_bind(IN osm_opensm_t * p_osm, IN ib_net64_t guid)
{
	ib_api_status_t status;

	OSM_LOG_ENTER(&p_osm->log);

	status = osm_sm_bind(&p_osm->sm, guid);
	if (status != IB_SUCCESS)
		goto Exit;

	status = osm_sa_bind(&p_osm->sa, guid);
	if (status != IB_SUCCESS)
		goto Exit;

#ifdef ENABLE_OSM_PERF_MGR
	status = osm_perfmgr_bind(&p_osm->perfmgr, guid);
	if (status != IB_SUCCESS)
		goto Exit;
#endif				/* ENABLE_OSM_PERF_MGR */

	status = osm_congestion_control_bind(&p_osm->cc, guid);
	if (status != IB_SUCCESS)
		goto Exit;

	/* setting IS_SM in capability mask */
	OSM_LOG(&p_osm->log, OSM_LOG_INFO, "Setting IS_SM on port 0x%016" PRIx64 "\n",
			cl_ntoh64(guid));
	osm_vendor_set_sm(p_osm->sm.mad_ctrl.h_bind, TRUE);

Exit:
	OSM_LOG_EXIT(&p_osm->log);
	return status;
}