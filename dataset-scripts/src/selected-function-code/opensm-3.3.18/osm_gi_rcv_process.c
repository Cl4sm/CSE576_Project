void osm_gi_rcv_process(IN void *context, IN void *data)
{
	osm_sm_t *sm = context;
	osm_madw_t *p_madw = data;
	ib_guid_info_t *p_gi;
	ib_smp_t *p_smp;
	osm_port_t *p_port;
	osm_gi_context_t *p_context;
	ib_net64_t port_guid, node_guid;
	uint8_t block_num;

	CL_ASSERT(sm);

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_madw);

	p_smp = osm_madw_get_smp_ptr(p_madw);
	p_context = osm_madw_get_gi_context_ptr(p_madw);
	p_gi = ib_smp_get_payload_ptr(p_smp);

	CL_ASSERT(p_smp->attr_id == IB_MAD_ATTR_GUID_INFO);

	block_num = (uint8_t) cl_ntoh32(p_smp->attr_mod);

	port_guid = p_context->port_guid;
	node_guid = p_context->node_guid;

	osm_dump_guid_info_v2(sm->p_log, node_guid, port_guid, block_num, p_gi,
			      FILE_ID, OSM_LOG_DEBUG);

	if (ib_smp_get_status(p_smp)) {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"MAD status 0x%x received\n",
			cl_ntoh16(ib_smp_get_status(p_smp)));
		goto Exit;
	}

	CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);
	p_port = osm_get_port_by_guid(sm->p_subn, port_guid);
	if (!p_port) {
		CL_PLOCK_RELEASE(sm->p_lock);
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 4701: "
			"No port object for port with GUID 0x%" PRIx64
			"\n\t\t\t\tfor parent node GUID 0x%" PRIx64
			", TID 0x%" PRIx64 "\n",
			cl_ntoh64(port_guid), cl_ntoh64(node_guid),
			cl_ntoh64(p_smp->trans_id));
		goto Exit;
	}

	CL_PLOCK_RELEASE(sm->p_lock);

Exit:
	/*
	   Release the lock before jumping here!!
	 */
	OSM_LOG_EXIT(sm->p_log);
}