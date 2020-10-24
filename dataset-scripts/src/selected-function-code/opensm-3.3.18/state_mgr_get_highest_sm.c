static osm_remote_sm_t *state_mgr_get_highest_sm(IN osm_sm_t * sm)
{
	cl_qmap_t *p_sm_tbl;
	osm_remote_sm_t *p_sm = NULL;
	osm_remote_sm_t *p_highest_sm;
	uint8_t highest_sm_priority;
	ib_net64_t highest_sm_guid;
	osm_node_t *p_node;

	OSM_LOG_ENTER(sm->p_log);

	p_sm_tbl = &sm->p_subn->sm_guid_tbl;

	/* Start with the local sm as the standard */
	p_highest_sm = NULL;
	highest_sm_priority = sm->p_subn->opt.sm_priority;
	highest_sm_guid = sm->p_subn->sm_port_guid;

	/* go over all the remote SMs */
	for (p_sm = (osm_remote_sm_t *) cl_qmap_head(p_sm_tbl);
	     p_sm != (osm_remote_sm_t *) cl_qmap_end(p_sm_tbl);
	     p_sm = (osm_remote_sm_t *) cl_qmap_next(&p_sm->map_item)) {

		/* If the sm is in NOTACTIVE state - continue */
		if (ib_sminfo_get_state(&p_sm->smi) ==
		    IB_SMINFO_STATE_NOTACTIVE)
			continue;

		if (osm_sm_is_greater_than(ib_sminfo_get_priority(&p_sm->smi),
					   p_sm->smi.guid, highest_sm_priority,
					   highest_sm_guid)) {
			/* the new p_sm is with higher priority - update the highest_sm */
			/* to this sm */
			p_highest_sm = p_sm;
			highest_sm_priority =
			    ib_sminfo_get_priority(&p_sm->smi);
			highest_sm_guid = p_sm->smi.guid;
		}
	}

	if (p_highest_sm != NULL) {
		p_node = osm_get_node_by_guid(sm->p_subn, p_highest_sm->smi.guid);
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Found higher priority SM with guid: %016" PRIx64 " (node %s)\n",
			cl_ntoh64(p_highest_sm->smi.guid),
			p_node ? p_node->print_desc : "UNKNOWN");
	}
	OSM_LOG_EXIT(sm->p_log);
	return p_highest_sm;
}