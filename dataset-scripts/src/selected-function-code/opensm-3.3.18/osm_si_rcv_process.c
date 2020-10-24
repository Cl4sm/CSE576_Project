void osm_si_rcv_process(IN void *context, IN void *data)
{
	osm_sm_t *sm = context;
	osm_madw_t *p_madw = data;
	ib_switch_info_t *p_si;
	ib_smp_t *p_smp;
	osm_node_t *p_node;
	ib_net64_t node_guid;
	osm_si_context_t *p_context;

	CL_ASSERT(sm);

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_madw);

	p_smp = osm_madw_get_smp_ptr(p_madw);
	p_si = ib_smp_get_payload_ptr(p_smp);
	p_context = osm_madw_get_si_context_ptr(p_madw);
	node_guid = p_context->node_guid;

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
		"Switch GUID 0x%016" PRIx64 ", TID 0x%" PRIx64 "\n",
		cl_ntoh64(node_guid), cl_ntoh64(p_smp->trans_id));

	if (ib_smp_get_status(p_smp)) {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"MAD status 0x%x received\n",
			cl_ntoh16(ib_smp_get_status(p_smp)));
		goto Exit2;
	}

	CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);

	p_node = osm_get_node_by_guid(sm->p_subn, node_guid);
	if (!p_node) {
		CL_PLOCK_RELEASE(sm->p_lock);
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3606: "
			"SwitchInfo received for nonexistent node "
			"with GUID 0x%" PRIx64 "\n", cl_ntoh64(node_guid));
		goto Exit;
	}

	/* Hack for bad value in Mellanox switch */
	if (cl_ntoh16(p_si->lin_top) > IB_LID_UCAST_END_HO) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3610: "
			"\n\t\t\t\tBad LinearFDBTop value = 0x%X "
			"on switch 0x%" PRIx64
			"\n\t\t\t\tForcing internal correction to 0x%X\n",
			cl_ntoh16(p_si->lin_top),
			cl_ntoh64(osm_node_get_node_guid(p_node)), 0);
		p_si->lin_top = 0;
	}

	/* Acquire the switch object for this switch. */
	if (!p_node->sw) {
		si_rcv_process_new(sm, p_node, p_madw);
		/* A new switch was found during the sweep so we need
		   to ignore the current LFT settings. */
		sm->p_subn->ignore_existing_lfts = TRUE;
	} else if (si_rcv_process_existing(sm, p_node, p_madw))
		/* we might get back a request for signaling change was detected */
		sm->p_subn->force_heavy_sweep = TRUE;

	if (p_context->light_sweep || p_context->set_method)
		goto Exit;

	si_rcv_get_sp0_info(sm, p_node);

Exit:
	CL_PLOCK_RELEASE(sm->p_lock);
Exit2:
	OSM_LOG_EXIT(sm->p_log);
}