ib_api_status_t
osmv_dispatch_mad(IN osm_bind_handle_t h_bind,
		  IN const void *p_mad_buf,
		  IN const osm_mad_addr_t * p_mad_addr)
{
	ib_api_status_t ret = IB_SUCCESS;
	const ib_mad_t *p_mad = (ib_mad_t *) p_mad_buf;
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	osmv_txn_ctx_t *p_txn = NULL;
	osm_log_t *p_log = p_bo->p_vendor->p_log;

	OSM_LOG_ENTER(p_bo->p_vendor->p_log);

	CL_ASSERT(NULL != h_bind && NULL != p_mad && NULL != p_mad_addr);

	osmv_txn_lock(p_bo);

	if (TRUE == p_bo->is_closing) {

		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"The bind handle %p is being closed. "
			"The MAD will not be dispatched.\n", p_bo);

		ret = IB_INTERRUPTED;
		goto dispatch_mad_done;
	}

	/*
	   Add call for packet drop randomizer.
	   This is a testing feature. If run_randomizer flag is set to TRUE,
	   the randomizer will be called, and randomally will drop
	   a packet. This is used for simulating unstable fabric.
	 */
	if (p_bo->p_vendor->run_randomizer == TRUE) {
		/* Try the randomizer */
		if (osm_pkt_randomizer_mad_drop(p_bo->p_vendor->p_log,
						p_bo->p_vendor->
						p_pkt_randomizer,
						p_mad) == TRUE) {
			osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
				"The MAD will not be dispatched.\n");
			goto dispatch_mad_done;
		}
	}

	switch (__osmv_dispatch_route(h_bind, p_mad, &p_txn)) {

	case OSMV_ROUTE_DROP:
		break;		/* Do nothing */

	case OSMV_ROUTE_SIMPLE:
		__osmv_dispatch_simple_mad(h_bind, p_mad, p_txn, p_mad_addr);
		break;

	case OSMV_ROUTE_RMPP:
		__osmv_dispatch_rmpp_mad(h_bind, p_mad, p_txn, p_mad_addr);
		break;

	default:
		CL_ASSERT(FALSE);
	}

dispatch_mad_done:
	osmv_txn_unlock(p_bo);

	OSM_LOG_EXIT(p_log);
	return ret;
}