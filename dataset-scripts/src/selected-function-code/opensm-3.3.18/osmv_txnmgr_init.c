ib_api_status_t
osmv_txnmgr_init(IN osmv_txn_mgr_t * p_tx_mgr,
		 IN osm_log_t * p_log, IN cl_spinlock_t * p_lock)
{
	cl_status_t cl_st = CL_SUCCESS;

	p_tx_mgr->p_event_wheel = malloc(sizeof(cl_event_wheel_t));
	if (!p_tx_mgr->p_event_wheel) {
		return IB_INSUFFICIENT_MEMORY;
	}

	memset(p_tx_mgr->p_event_wheel, 0, sizeof(cl_event_wheel_t));

	cl_event_wheel_construct(p_tx_mgr->p_event_wheel);

	/* NOTE! We are using an extended constructor.
	 * We tell the Event Wheel run in a non-protected manner in the reg/unreg calls,
	 * and acquire an external lock in the asynchronous callback.
	 */
	cl_st = cl_event_wheel_init_ex(p_tx_mgr->p_event_wheel, p_lock);
	if (cl_st != CL_SUCCESS) {
		free(p_tx_mgr->p_event_wheel);
		return (ib_api_status_t) cl_st;
	}

	p_tx_mgr->p_txn_map = malloc(sizeof(cl_qmap_t));
	if (!p_tx_mgr->p_txn_map) {
		cl_event_wheel_destroy(p_tx_mgr->p_event_wheel);
		free(p_tx_mgr->p_event_wheel);
		return IB_INSUFFICIENT_MEMORY;
	}

	memset(p_tx_mgr->p_txn_map, 0, sizeof(cl_qmap_t));

	cl_qmap_init(p_tx_mgr->p_txn_map);
	p_tx_mgr->p_log = p_log;

	return cl_st;
}