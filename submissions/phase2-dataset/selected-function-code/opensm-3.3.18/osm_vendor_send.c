ib_api_status_t
osm_vendor_send(IN osm_bind_handle_t h_bind,
		IN osm_madw_t * const p_madw, IN boolean_t const resp_expected)
{
	ib_api_status_t ret = IB_SUCCESS;
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	boolean_t is_rmpp = FALSE, is_rmpp_ds = FALSE;
	osmv_txn_ctx_t *p_txn = NULL;
	ib_mad_t *p_mad;
	osm_log_t *p_log = p_bo->p_vendor->p_log;
	osm_mad_pool_t *p_mad_pool = p_bo->p_osm_pool;
	OSM_LOG_ENTER(p_log);

	if (NULL == h_bind || NULL == p_madw ||
	    NULL == (p_mad = osm_madw_get_mad_ptr(p_madw)) ||
	    NULL == osm_madw_get_mad_addr_ptr(p_madw)) {

		return IB_INVALID_PARAMETER;
	}

	is_rmpp = (p_madw->mad_size > MAD_BLOCK_SIZE
		   || osmv_mad_is_rmpp(p_mad));
	/* is this rmpp double sided? This means we expect a response that can be
	   an rmpp or not */
	is_rmpp_ds = (TRUE == is_rmpp && TRUE == resp_expected);

	/* Make our operations with the send context atomic */
	osmv_txn_lock(p_bo);

	if (TRUE == p_bo->is_closing) {

		osm_log(p_log, OSM_LOG_ERROR,
			"osm_vendor_send: ERR 7310: "
			"The handle %p is being unbound, cannot send.\n",
			h_bind);
		ret = IB_INTERRUPTED;
		/* When closing p_bo could be detroyed or is going to , thus could not refer to it */
		goto send_done;
	}

	if (TRUE == resp_expected || TRUE == is_rmpp) {

		/* We must run under a transaction framework.
		 * Get the transaction object (old or new) */
		ret = __osmv_get_send_txn(h_bind, p_madw, is_rmpp,
					  resp_expected, &p_txn);
		if (IB_SUCCESS != ret) {
			goto send_done;
		}
	}

	if (TRUE == is_rmpp) {
		/* Do the job - RMPP!
		 * The call returns as all the packets are ACK'ed/upon error
		 * The txn lock will be released each time the function sleeps
		 * and re-acquired when it wakes up
		 */
		ret = osmv_rmpp_send_madw(h_bind, p_madw, p_txn, is_rmpp_ds);
	} else {

		/* Do the job - single MAD!
		 * The call returns as soon as the MAD is put on the wire
		 */
		ret = osmv_simple_send_madw(h_bind, p_madw, p_txn, FALSE);
	}

	if (IB_SUCCESS == ret) {

		if ((TRUE == is_rmpp) && (FALSE == is_rmpp_ds)) {
			/* For double-sided sends, the txn continues to live */
			osmv_txn_done(h_bind, osmv_txn_get_key(p_txn),
				      FALSE /*not in callback */ );
		}

		if (FALSE == resp_expected) {
			osm_mad_pool_put(p_mad_pool, p_madw);
		}
	} else if (IB_INTERRUPTED != ret) {
		if (NULL != p_txn) {
			osmv_txn_done(h_bind, osmv_txn_get_key(p_txn),
				      FALSE /*not in callback */ );
		}

		osm_log(p_log, OSM_LOG_ERROR,
			"osm_vendor_send: ERR 7311: failed to send MADW %p\n",
			p_madw);

		if (TRUE == resp_expected) {
			/* Change the status on the p_madw */
			p_madw->status = ret;
			/* Only the requester expects the error callback */
			p_bo->send_err_cb(p_bo->cb_context, p_madw);
		} else {
			/* put back the mad - it is useless ... */
			osm_mad_pool_put(p_mad_pool, p_madw);
		}
	} else {		/* the transaction was aborted due to p_bo exit */

		osm_mad_pool_put(p_mad_pool, p_madw);
		goto aborted;
	}
send_done:

	osmv_txn_unlock(p_bo);
aborted:
	OSM_LOG_EXIT(p_log);
	return ret;
}