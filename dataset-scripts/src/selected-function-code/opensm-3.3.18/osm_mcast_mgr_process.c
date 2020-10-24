int osm_mcast_mgr_process(osm_sm_t * sm, boolean_t config_all)
{
	int ret = 0;
	unsigned i;
	unsigned max_mlid;

	OSM_LOG_ENTER(sm->p_log);

	CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);

	/* If there are no switches in the subnet we have nothing to do. */
	if (cl_qmap_count(&sm->p_subn->sw_guid_tbl) == 0) {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"No switches in subnet. Nothing to do\n");
		goto exit;
	}

	if (alloc_mfts(sm)) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR,
			"ERR 0A09: alloc_mfts failed\n");
		ret = -1;
		goto exit;
	}

	max_mlid = config_all ? sm->p_subn->max_mcast_lid_ho
			- IB_LID_MCAST_START_HO : sm->mlids_req_max;
	for (i = 0; i <= max_mlid; i++) {
		if (sm->mlids_req[i] ||
		    (config_all && sm->p_subn->mboxes[i])) {
			sm->mlids_req[i] = 0;
			mcast_mgr_process_mlid(sm, i + IB_LID_MCAST_START_HO);
		}
	}

	sm->mlids_req_max = 0;

	ret = mcast_mgr_set_mftables(sm);

	osm_dump_mcast_routes(sm->p_subn->p_osm);

exit:
	CL_PLOCK_RELEASE(sm->p_lock);
	OSM_LOG_EXIT(sm->p_log);
	return ret;
}