ib_api_status_t
osmv_txn_init_rmpp_sender(IN osm_bind_handle_t h_bind,
			  IN osmv_txn_ctx_t * p_txn, IN osm_madw_t * p_madw)
{
	ib_api_status_t st;

	CL_ASSERT(p_txn);

	/* Double-Sided RMPP Direction Switch */
	osmv_txn_remove_timeout_ev(h_bind, osmv_txn_get_key(p_txn));

	p_txn->rmpp_txfr.rmpp_state = OSMV_TXN_RMPP_SENDER;
	p_txn->rmpp_txfr.p_rmpp_send_ctx = malloc(sizeof(osmv_rmpp_send_ctx_t));

	if (!p_txn->rmpp_txfr.p_rmpp_send_ctx) {
		return IB_INSUFFICIENT_MEMORY;
	}

	memset(p_txn->rmpp_txfr.p_rmpp_send_ctx, 0,
	       sizeof(osmv_rmpp_send_ctx_t));

	st = osmv_rmpp_send_ctx_init(p_txn->rmpp_txfr.p_rmpp_send_ctx,
				     (void *)p_madw->p_mad,
				     p_madw->mad_size, p_txn->p_log);
	return st;
}