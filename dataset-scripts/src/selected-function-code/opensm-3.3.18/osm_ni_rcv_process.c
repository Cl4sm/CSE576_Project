void osm_ni_rcv_process(IN void *context, IN void *data)
{
	osm_sm_t *sm = context;
	osm_madw_t *p_madw = data;
	ib_node_info_t *p_ni;
	ib_smp_t *p_smp;
	osm_node_t *p_node;

	CL_ASSERT(sm);

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_madw);

	p_smp = osm_madw_get_smp_ptr(p_madw);
	p_ni = ib_smp_get_payload_ptr(p_smp);

	CL_ASSERT(p_smp->attr_id == IB_MAD_ATTR_NODE_INFO);

	if (PF(p_ni->node_guid == 0)) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D16: "
			"Got Zero Node GUID! Found on the directed route:\n");
		osm_dump_smp_dr_path_v2(sm->p_log, p_smp, FILE_ID, OSM_LOG_ERROR);
		goto Exit;
	}

	if (PF(p_ni->port_guid == 0)) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D17: "
			"Got Zero Port GUID! Found on the directed route:\n");
		osm_dump_smp_dr_path_v2(sm->p_log, p_smp, FILE_ID, OSM_LOG_ERROR);
		goto Exit;
	}

	if (ib_smp_get_status(p_smp)) {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"MAD status 0x%x received\n",
			cl_ntoh16(ib_smp_get_status(p_smp)));
		goto Exit;
	}

	/*
	   Determine if this node has already been discovered,
	   and process accordingly.
	   During processing of this node, hold the shared lock.
	 */

	CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);
	p_node = osm_get_node_by_guid(sm->p_subn, p_ni->node_guid);

	osm_dump_node_info_v2(sm->p_log, p_ni, FILE_ID, OSM_LOG_DEBUG);

	if (!p_node)
		ni_rcv_process_new(sm, p_madw);
	else
		ni_rcv_process_existing(sm, p_node, p_madw);

	CL_PLOCK_RELEASE(sm->p_lock);

Exit:
	OSM_LOG_EXIT(sm->p_log);
}