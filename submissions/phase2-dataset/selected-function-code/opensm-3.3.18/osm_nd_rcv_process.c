void osm_nd_rcv_process(IN void *context, IN void *data)
{
	osm_sm_t *sm = context;
	osm_madw_t *p_madw = data;
	ib_node_desc_t *p_nd;
	ib_smp_t *p_smp;
	osm_node_t *p_node;
	ib_net64_t node_guid;

	CL_ASSERT(sm);

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_madw);

	p_smp = osm_madw_get_smp_ptr(p_madw);
	if (ib_smp_get_status(p_smp)) {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"MAD status 0x%x received\n",
			cl_ntoh16(ib_smp_get_status(p_smp)));
		goto Exit;
	}

	p_nd = ib_smp_get_payload_ptr(p_smp);

	/* Acquire the node object and add the node description. */
	node_guid = osm_madw_get_nd_context_ptr(p_madw)->node_guid;
	CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);
	p_node = osm_get_node_by_guid(sm->p_subn, node_guid);
	if (!p_node)
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0B01: "
			"NodeDescription received for nonexistent node "
			"0x%" PRIx64 "\n", cl_ntoh64(node_guid));
	else
		nd_rcv_process_nd(sm, p_node, p_nd);

	CL_PLOCK_RELEASE(sm->p_lock);
Exit:
	OSM_LOG_EXIT(sm->p_log);
}