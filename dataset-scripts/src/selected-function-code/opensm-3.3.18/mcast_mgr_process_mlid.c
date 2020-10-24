static ib_api_status_t mcast_mgr_process_mlid(osm_sm_t * sm, uint16_t mlid)
{
	ib_api_status_t status = IB_SUCCESS;
	struct osm_routing_engine *re = sm->p_subn->p_osm->routing_engine_used;
	osm_mgrp_box_t *mbox;

	OSM_LOG_ENTER(sm->p_log);

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
		"Processing multicast group with mlid 0x%X\n", mlid);

	/* Clear the multicast tables to start clean, then build
	   the spanning tree which sets the mcast table bits for each
	   port in the group. */
	mcast_mgr_clear(sm, mlid);

	mbox = osm_get_mbox_by_mlid(sm->p_subn, cl_hton16(mlid));
	if (mbox) {
		if (re && re->mcast_build_stree)
			status = re->mcast_build_stree(re->context, mbox);
		else
			status = mcast_mgr_build_spanning_tree(sm, mbox);

		if (status != IB_SUCCESS)
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0A17: "
				"Unable to create spanning tree (%s) for mlid "
				"0x%x\n", ib_get_err_str(status), mlid);
	}

	OSM_LOG_EXIT(sm->p_log);
	return status;
}