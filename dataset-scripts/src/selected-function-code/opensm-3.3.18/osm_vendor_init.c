ib_api_status_t
osm_vendor_init(IN osm_vendor_t * const p_vend,
		IN osm_log_t * const p_log, IN const uint32_t timeout)
{
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(p_log);

	p_vend->p_transport_info = NULL;
	p_vend->p_log = p_log;
	p_vend->resp_timeout = timeout;
	p_vend->ttime_timeout = timeout * OSMV_TXN_TIMEOUT_FACTOR;

	cl_qlist_init(&p_vend->bind_handles);

	/* update the run_randomizer flag */
	if (getenv("OSM_PKT_DROP_RATE") != NULL
	    && atol(getenv("OSM_PKT_DROP_RATE")) != 0) {
		/* if the OSM_PKT_DROP_RATE global variable is defined to a non-zero value -
		   then the randomizer should be called.
		   Need to create the packet randomizer object */
		p_vend->run_randomizer = TRUE;
		status =
		    osm_pkt_randomizer_init(&(p_vend->p_pkt_randomizer), p_log);
		if (status != IB_SUCCESS)
			return status;
	} else {
		p_vend->run_randomizer = FALSE;
		p_vend->p_pkt_randomizer = NULL;
	}

	OSM_LOG_EXIT(p_log);
	return (IB_SUCCESS);
}