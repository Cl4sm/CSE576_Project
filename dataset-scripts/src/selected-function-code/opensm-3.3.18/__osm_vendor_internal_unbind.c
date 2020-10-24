static void __osm_vendor_internal_unbind(osm_bind_handle_t h_bind)
{
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	osm_log_t *p_log = p_bo->p_vendor->p_log;

	OSM_LOG_ENTER(p_log);

	/* "notifying" all that from now on no new sends can be done */
	p_bo->txn_mgr.p_event_wheel->closing = TRUE;

	osmv_txn_lock(p_bo);

	/*
	   the is_closing is set under lock we we know we only need to
	   check for it after obtaining the lock
	 */
	p_bo->is_closing = TRUE;

	/* notifying all sleeping rmpp sends to exit */
	osmv_txn_abort_rmpp_txns(h_bind);

	/* unlock the bo to allow for any residual mads to be dispatched */
	osmv_txn_unlock(p_bo);
	osm_log(p_log, OSM_LOG_DEBUG,
		"__osm_vendor_internal_unbind: destroying transport mgr.. \n");
	/* wait for the receiver thread to exit */
	osmv_transport_done(h_bind);

	/* lock to avoid any collissions while we cleanup the structs */
	osmv_txn_lock(p_bo);
	osm_log(p_log, OSM_LOG_DEBUG,
		"__osm_vendor_internal_unbind: destroying txn mgr.. \n");
	osmv_txnmgr_done(h_bind);
	osm_log(p_log, OSM_LOG_DEBUG,
		"__osm_vendor_internal_unbind: destroying bind lock.. \n");
	osmv_txn_unlock(p_bo);

	/*
	   we intentionally let the p_bo and its lock leak -
	   as we did not implement a way to track active bind handles provided to
	   the client - and the client might use them

	   cl_spinlock_destroy(&p_bo->lock);
	   free(p_bo);
	 */

	OSM_LOG_EXIT(p_log);
}