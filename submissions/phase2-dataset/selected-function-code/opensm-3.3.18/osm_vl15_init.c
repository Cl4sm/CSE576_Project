ib_api_status_t osm_vl15_init(IN osm_vl15_t * p_vl, IN osm_vendor_t * p_vend,
			      IN osm_log_t * p_log, IN osm_stats_t * p_stats,
			      IN osm_subn_t * p_subn,
			      IN int32_t max_wire_smps,
			      IN int32_t max_wire_smps2,
			      IN uint32_t max_smps_timeout)
{
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(p_log);

	p_vl->p_vend = p_vend;
	p_vl->p_log = p_log;
	p_vl->p_stats = p_stats;
	p_vl->p_subn = p_subn;
	p_vl->max_wire_smps = max_wire_smps;
	p_vl->max_wire_smps2 = max_wire_smps2;
	p_vl->max_smps_timeout = max_wire_smps < max_wire_smps2 ?
				 max_smps_timeout : EVENT_NO_TIMEOUT;

	status = cl_event_init(&p_vl->signal, FALSE);
	if (status != IB_SUCCESS)
		goto Exit;

	p_vl->state = OSM_VL15_STATE_READY;

	status = cl_spinlock_init(&p_vl->lock);
	if (status != IB_SUCCESS)
		goto Exit;

	/*
	   Initialize the thread after all other dependent objects
	   have been initialized.
	 */
	status = cl_thread_init(&p_vl->poller, vl15_poller, p_vl,
				"opensm poller");
Exit:
	OSM_LOG_EXIT(p_log);
	return status;
}