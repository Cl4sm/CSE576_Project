ib_api_status_t osm_congestion_control_init(osm_congestion_control_t * p_cc,
					    struct osm_opensm *p_osm,
					    const osm_subn_opt_t * p_opt)
{
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(&p_osm->log);

	memset(p_cc, 0, sizeof(*p_cc));

	p_cc->osm = p_osm;
	p_cc->subn = &p_osm->subn;
	p_cc->sm = &p_osm->sm;
	p_cc->log = &p_osm->log;
	p_cc->mad_pool = &p_osm->mad_pool;
	p_cc->trans_id = CONGESTION_CONTROL_INITIAL_TID_VALUE;
	p_cc->vendor = p_osm->p_vendor;

	p_cc->cc_disp_h = cl_disp_register(&p_osm->disp, OSM_MSG_MAD_CC,
					   cc_rcv_mad, p_cc);
	if (p_cc->cc_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	cl_qlist_init(&p_cc->mad_queue);

	status = cl_spinlock_init(&p_cc->mad_queue_lock);
	if (status != IB_SUCCESS)
		goto Exit;

	cl_event_construct(&p_cc->cc_poller_wakeup);
	status = cl_event_init(&p_cc->cc_poller_wakeup, FALSE);
	if (status != IB_SUCCESS)
		goto Exit;

	cl_event_construct(&p_cc->outstanding_mads_done_event);
	status = cl_event_init(&p_cc->outstanding_mads_done_event, FALSE);
	if (status != IB_SUCCESS)
		goto Exit;

	cl_event_construct(&p_cc->sig_mads_on_wire_continue);
	status = cl_event_init(&p_cc->sig_mads_on_wire_continue, FALSE);
	if (status != IB_SUCCESS)
		goto Exit;

	p_cc->thread_state = OSM_THREAD_STATE_NONE;

	status = cl_thread_init(&p_cc->cc_poller, cc_poller, p_cc,
				"cc poller");
	if (status != IB_SUCCESS)
		goto Exit;

	status = IB_SUCCESS;
Exit:
	OSM_LOG_EXIT(p_cc->log);
	return status;
}