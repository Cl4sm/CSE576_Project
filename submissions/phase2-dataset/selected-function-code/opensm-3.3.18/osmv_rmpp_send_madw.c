ib_api_status_t
osmv_rmpp_send_madw(IN osm_bind_handle_t h_bind,
		    IN osm_madw_t * const p_madw,
		    IN osmv_txn_ctx_t * p_txn, IN boolean_t is_rmpp_ds)
{
	ib_api_status_t ret = IB_SUCCESS;
	uint32_t i, total_segs;

	osmv_rmpp_send_ctx_t *p_send_ctx = osmv_txn_get_rmpp_send_ctx(p_txn);
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;

	OSM_LOG_ENTER(p_bo->p_vendor->p_log);

	total_segs = osmv_rmpp_send_ctx_get_num_segs(p_send_ctx);
	CL_ASSERT(total_segs >= 1);

	/* In the double-sided transfer, wait for ACK 0 */

	for (;;) {

		if (p_send_ctx->window_first > total_segs) {

			/* Every segment is acknowledged */
			break;
		}

		/* Send the next burst. */
		for (i = p_send_ctx->window_first; i <= p_send_ctx->window_last;
		     i++) {

			/* Send a segment and setup a timeout timer */
			ret = __osmv_rmpp_send_segment(h_bind, p_txn, i);
			if (IB_SUCCESS != ret) {
				goto send_done;
			}
		}

		/* Set the Response Timeout for the ACK on the last DATA segment */
		ret = osmv_txn_set_timeout_ev(h_bind, osmv_txn_get_key(p_txn),
					      p_bo->p_vendor->resp_timeout);
		if (IB_SUCCESS != ret) {
			goto send_done;
		}

		/* Going to sleep. Let the others access the transaction DB */
		osmv_txn_unlock(p_bo);

		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"RMPP Sender thread (madw=%p) going to sleep ...\n",
			p_madw);

		/* Await the next event to happen */
		cl_event_wait_on(&p_send_ctx->event,
				 EVENT_NO_TIMEOUT, TRUE /* interruptible */ );

		/* Got a signal from the MAD dispatcher/timeout handler */
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"RMPP Sender thread (madw=%p) waking up on a signal ...\n",
			p_madw);

		/* Let's see what changed... Make this atomic - re-acquire the lock. */
		osmv_txn_lock(p_bo);

		if (TRUE == p_bo->is_closing) {
			osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
				"osmv_rmpp_send_madw: ERR 6601: "
				"The bind handle %p is being closed. "
				"Stopping the RMPP Send of MADW %p\n",
				h_bind, p_madw);

			ret = IB_TIMEOUT;
			return IB_INTERRUPTED;
		}

		/* STOP? ABORT? TIMEOUT? */
		if (IB_SUCCESS != p_send_ctx->status) {
			osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
				"osmv_rmpp_send_madw: ERR 6602: "
				"An error (%s) happened during the RMPP send of %p. Bailing out.\n",
				ib_get_err_str(p_send_ctx->status), p_madw);
			ret = p_send_ctx->status;
			goto send_done;
		}
	}

	if (TRUE == is_rmpp_ds) {
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"Double-sided RMPP - switching to be the receiver.\n");

		ret = osmv_txn_init_rmpp_receiver(h_bind, p_txn, FALSE
						  /*Send was initiated by me */
						  );

		if (IB_SUCCESS == ret) {
			/* Send ACK on the 0 segment */
			ret = __osmv_rmpp_send_segment(h_bind, p_txn, 0);
		}
	}

send_done:
	OSM_LOG_EXIT(p_bo->p_vendor->p_log);
	return ret;
}