static uint64_t
__osmv_txn_timeout_cb(IN uint64_t key,
		      IN uint32_t num_regs, IN void *cb_context)
{
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) cb_context;
	uint64_t ret = 0;
	osmv_txn_ctx_t *p_txn;
	osmv_rmpp_send_ctx_t *p_send_ctx;
	osm_madw_t *p_madw = NULL;
	ib_mad_t *p_mad;
	osm_mad_addr_t *p_mad_addr;
	boolean_t invoke_err_cb = FALSE;

	OSM_LOG_ENTER(p_bo->p_vendor->p_log);

	/* Don't try to acquire a lock on the Bind Object -
	 * it's taken by the mechanism that drives the timeout based events!
	 * (Recall the special constructor that the Event Wheel is applied with)
	 */
	if (p_bo->is_closing) {
		goto txn_done;
	}

	ret = osmv_txn_lookup(p_bo, key, &p_txn);
	if (IB_NOT_FOUND == ret) {
		/* Prevent a race - the transaction is already destroyed */
		goto txn_done;
	}

	p_madw = p_txn->p_madw;

	switch (osmv_txn_get_rmpp_state(p_txn)) {

	case OSMV_TXN_RMPP_NONE:
		if (num_regs <= OSM_DEFAULT_RETRY_COUNT) {
			/* We still did not exceed the limit of retransmissions.
			 * Set the next timeout's value.
			 */
			osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
				"__osmv_txn_timeout_cb: "
				"The transaction request (tid=0x%016" PRIx64 ")"
				" timed out %d times. Retrying the send.\n",
				osmv_txn_get_tid(p_txn), num_regs);

			/* resend this mad */
			ret = osmv_simple_send_madw((osm_bind_handle_t *) p_bo,
						    p_madw, p_txn, TRUE);
			if (ret != IB_SUCCESS) {
				osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
					"__osmv_txn_timeout_cb: "
					"Fail to send retry for transaction"
					"request (tid=0x%016" PRIx64 ").\n",
					osmv_txn_get_tid(p_txn));

				osmv_txn_done((osm_bind_handle_t) p_bo, key,
					      TRUE /*in timeout callback */ );

				/* This is a requester. Always apply the callback */
				invoke_err_cb = TRUE;
			} else {
				uint64_t next_timeout_ms;
				next_timeout_ms =
				    p_bo->p_vendor->resp_timeout * (num_regs +
								    1) *
				    (num_regs + 1);
				/* when do we need to timeout again */
				ret =
				    cl_get_time_stamp() +
				    (uint64_t) (1000 * next_timeout_ms);

				osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
					"__osmv_txn_timeout_cb: "
					"Retry request timout in : %lu [msec].\n",
					next_timeout_ms);
			}
		} else {
			osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
				"__osmv_txn_timeout_cb: ERR 6702: "
				"The transaction request (0x%016" PRIx64 ") "
				"timed out (after %d retries). "
				"Invoking the error callback.\n",
				osmv_txn_get_tid(p_txn), num_regs);

			osmv_txn_done((osm_bind_handle_t) p_bo, key,
				      TRUE /*in timeout callback */ );

			/* This is a requester. Always apply the callback */
			invoke_err_cb = TRUE;
		}
		break;

	case OSMV_TXN_RMPP_SENDER:
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"RMPP sender (tid=0x%016" PRIx64 ") did not receive ACK "
			"on every segment in the current send window.\n",
			osmv_txn_get_tid(p_txn));

		p_send_ctx = osmv_txn_get_rmpp_send_ctx(p_txn);
		if (num_regs <= OSM_DEFAULT_RETRY_COUNT) {
			/* We still did not exceed the limit of retransmissions.
			 * Set the next timeout's value.
			 */
			ret =
			    cl_get_time_stamp() +
			    1000 * p_bo->p_vendor->resp_timeout;
		} else {
			p_send_ctx->status = IB_TIMEOUT;

			p_mad = osm_madw_get_mad_ptr(p_madw);
			p_mad_addr = osm_madw_get_mad_addr_ptr(p_madw);

			/* Send an ABORT to the other side */
			osmv_rmpp_send_nak((osm_bind_handle_t) p_bo, p_mad,
					   p_mad_addr, IB_RMPP_TYPE_ABORT,
					   IB_RMPP_STATUS_T2L);
		}

		/* Wake the RMPP sender thread up */
		cl_event_signal(&p_send_ctx->event);
		break;

	case OSMV_TXN_RMPP_RECEIVER:
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"Transaction timeout on an RMPP receiver "
			"(tid=0x%016" PRIx64 "). Dropping the transaction.\n",
			osmv_txn_get_tid(p_txn));

		osmv_txn_done((osm_bind_handle_t) p_bo, key,
			      TRUE /*in timeout callback */ );

		if (FALSE == osmv_txn_is_rmpp_init_by_peer(p_txn)) {
			/* This is a requester, still waiting for the reply. Apply the callback */
			invoke_err_cb = TRUE;
		}

		break;

	default:
		CL_ASSERT(FALSE);
	}

	if (TRUE == invoke_err_cb) {
		CL_ASSERT(NULL != p_madw);
		/* update the status in the p_madw */
		p_madw->status = IB_TIMEOUT;
		p_bo->send_err_cb(p_bo->cb_context, p_madw);
		/* no re-registration */
		ret = 0;
	}

txn_done:
	OSM_LOG_EXIT(p_bo->p_vendor->p_log);
	return ret;
}