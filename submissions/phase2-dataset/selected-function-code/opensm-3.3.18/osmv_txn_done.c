void
osmv_txn_done(IN osm_bind_handle_t h_bind,
	      IN uint64_t key, IN boolean_t is_in_cb)
{
	osmv_txn_ctx_t *p_ctx;
	osmv_bind_obj_t *const p_bo = (osmv_bind_obj_t *) h_bind;

	OSM_LOG_ENTER(p_bo->p_vendor->p_log);

	CL_ASSERT(h_bind);

	/* Cancel the (single) timeout possibly outstanding for this txn
	 * Don't do this if you are in the callback context, for 2 reasons:
	 * (1) The event wheel will remove the context itself.
	 * (2) If we try to, there is a deadlock in the event wheel
	 */
	if (FALSE == is_in_cb) {
		osmv_txn_remove_timeout_ev(h_bind, key);
	}

	/* Remove from DB */
	if (IB_NOT_FOUND ==
	    __osmv_txnmgr_remove_txn(&p_bo->txn_mgr, key, &p_ctx)) {
		return;
	}

	/* Destroy the transaction's RMPP contexts
	 * (can be more than one in the case of double sided transfer)
	 */

	if (p_ctx->rmpp_txfr.p_rmpp_send_ctx) {
		osmv_rmpp_send_ctx_done(p_ctx->rmpp_txfr.p_rmpp_send_ctx);
	}

	if (p_ctx->rmpp_txfr.p_rmpp_recv_ctx) {
		osmv_rmpp_recv_ctx_done(p_ctx->rmpp_txfr.p_rmpp_recv_ctx);
	}

	free(p_ctx);

	OSM_LOG_EXIT(p_bo->p_vendor->p_log);
}