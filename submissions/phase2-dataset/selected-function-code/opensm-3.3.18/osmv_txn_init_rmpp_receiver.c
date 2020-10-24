ib_api_status_t
osmv_txn_init_rmpp_receiver(IN osm_bind_handle_t h_bind,
			    IN osmv_txn_ctx_t * p_txn,
			    IN boolean_t is_init_by_peer)
{
	ib_api_status_t st;
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	uint64_t key = osmv_txn_get_key(p_txn);

	CL_ASSERT(p_txn);

	/* Double-Sided RMPP Direction Switch */
	osmv_txn_remove_timeout_ev(h_bind, key);

	/* Set the Transaction Timeout value */
	st = osmv_txn_set_timeout_ev(h_bind, key,
				     p_bo->p_vendor->ttime_timeout);
	if (IB_SUCCESS != st) {

		return st;
	}

	p_txn->rmpp_txfr.rmpp_state = OSMV_TXN_RMPP_RECEIVER;
	p_txn->rmpp_txfr.is_rmpp_init_by_peer = is_init_by_peer;

	p_txn->rmpp_txfr.p_rmpp_recv_ctx = malloc(sizeof(osmv_rmpp_recv_ctx_t));

	if (!p_txn->rmpp_txfr.p_rmpp_recv_ctx) {

		osmv_txn_remove_timeout_ev(h_bind, key);
		return IB_INSUFFICIENT_MEMORY;
	}

	memset(p_txn->rmpp_txfr.p_rmpp_recv_ctx, 0,
	       sizeof(osmv_rmpp_recv_ctx_t));

	st = osmv_rmpp_recv_ctx_init(p_txn->rmpp_txfr.p_rmpp_recv_ctx,
				     p_txn->p_log);

	return st;
}