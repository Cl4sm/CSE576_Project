static osm_remote_sm_t *state_mgr_exists_other_master_sm(IN osm_sm_t * sm)
{
	cl_qmap_t *p_sm_tbl;
	osm_remote_sm_t *p_sm;
	osm_remote_sm_t *p_sm_res = NULL;
	osm_node_t *p_node;

	OSM_LOG_ENTER(sm->p_log);

	p_sm_tbl = &sm->p_subn->sm_guid_tbl;

	/* go over all the remote SMs */
	for (p_sm = (osm_remote_sm_t *) cl_qmap_head(p_sm_tbl);
	     p_sm != (osm_remote_sm_t *) cl_qmap_end(p_sm_tbl);
	     p_sm = (osm_remote_sm_t *) cl_qmap_next(&p_sm->map_item)) {
		/* If the sm is in MASTER state - return a pointer to it */
		p_node = osm_get_node_by_guid(sm->p_subn, p_sm->smi.guid);
		if (ib_sminfo_get_state(&p_sm->smi) == IB_SMINFO_STATE_MASTER) {
			OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
				"Found remote master SM with guid:0x%016" PRIx64
				" (node %s)\n", cl_ntoh64(p_sm->smi.guid),
				p_node ? p_node->print_desc : "UNKNOWN");
			p_sm_res = p_sm;
			goto Exit;
		}
	}

Exit:
	OSM_LOG_EXIT(sm->p_log);
	return p_sm_res;
}