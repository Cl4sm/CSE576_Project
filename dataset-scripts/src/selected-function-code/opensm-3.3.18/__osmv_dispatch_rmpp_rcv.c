static ib_api_status_t
__osmv_dispatch_rmpp_rcv(IN osm_bind_handle_t h_bind,
			 IN const ib_mad_t * p_mad,
			 IN osmv_txn_ctx_t * p_txn,
			 IN const osm_mad_addr_t * p_mad_addr)
{
	ib_api_status_t status = IB_SUCCESS;
	osmv_rmpp_recv_ctx_t *p_recv_ctx = osmv_txn_get_rmpp_recv_ctx(p_txn);
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	boolean_t is_last1 = FALSE, is_last2 = FALSE;
	osm_madw_t *p_new_madw = NULL, *p_req_madw = NULL;
	ib_mad_t *p_mad_buf;
	uint32_t size = 0;
	uint64_t key = osmv_txn_get_key(p_txn);
	uint64_t tid = osmv_txn_get_tid(p_txn);

	OSM_LOG_ENTER(p_bo->p_vendor->p_log);

	if (TRUE == osmv_rmpp_is_ack(p_mad)) {
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"Not supposed to receive ACK's --> dropping the MAD\n");

		goto dispatch_rmpp_rcv_done;
	}

	if (TRUE == osmv_rmpp_is_abort_stop(p_mad)) {
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"__osmv_dispatch_rmpp_rcv: ERR 6504: "
			"The Remote Side stopped sending\n");

		status = IB_REMOTE_ERROR;
		goto dispatch_rmpp_rcv_done;
	}

	status = __osmv_dispatch_accept_seg(h_bind, p_txn, p_mad);
	switch (status) {

	case IB_SUCCESS:

		/* Check wheter this is the legal last MAD */
		/* Criteria #1: the received MAD is marked last */
		is_last1 = osmv_rmpp_is_last(p_mad);

		/* Criteria #2: the total accumulated length hits the advertised one */
		is_last2 = is_last1;

		size = osmv_rmpp_recv_ctx_get_byte_num_from_first(p_recv_ctx);
		if (size > 0) {
			is_last2 =
			    (osmv_rmpp_recv_ctx_get_cur_byte_num(p_recv_ctx) >=
			     size);
		}

		if (is_last1 != is_last2) {

			osmv_rmpp_send_nak(h_bind, p_mad, p_mad_addr,
					   IB_RMPP_TYPE_ABORT,
					   IB_RMPP_STATUS_BAD_LEN);

			status = IB_ERROR;
			goto dispatch_rmpp_rcv_done;
		}

		/* TBD Consider an optimization - sending an ACK
		 * only for the last segment in the window
		 */
		__osmv_dispatch_send_ack(h_bind, p_mad, p_txn, p_mad_addr);
		break;

	case IB_INSUFFICIENT_RESOURCES:
		/* An out-of-order segment received. Send the ACK anyway */
		__osmv_dispatch_send_ack(h_bind, p_mad, p_txn, p_mad_addr);
		status = IB_SUCCESS;
		goto dispatch_rmpp_rcv_done;

	case IB_INSUFFICIENT_MEMORY:
		osmv_rmpp_send_nak(h_bind, p_mad, p_mad_addr,
				   IB_RMPP_TYPE_STOP, IB_RMPP_STATUS_RESX);
		goto dispatch_rmpp_rcv_done;

	default:
		/* Illegal return code */
		CL_ASSERT(FALSE);
	}

	if (TRUE != is_last1) {
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"RMPP MADW assembly continues, TID=0x%" PRIx64 "\n", tid);
		goto dispatch_rmpp_rcv_done;
	}

	/* This is the last packet. */
	if (0 == size) {
		/* The total size was not advertised in the first packet */
		size = osmv_rmpp_recv_ctx_get_byte_num_from_last(p_recv_ctx);
	}

	/*
	   NOTE: the received mad might not be >= 256 bytes.
	   some MADs might contain several SA records but still be
	   less then a full MAD.
	   We have to use RMPP to send them over since on a regular
	   "simple" MAD there is no way to know how many records were sent
	 */

	/* Build the MAD wrapper to be returned to the user.
	 * The actual storage for the MAD is allocated there.
	 */
	p_new_madw =
	    osm_mad_pool_get(p_bo->p_osm_pool, h_bind, size, p_mad_addr);
	if (NULL == p_new_madw) {
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
			"__osmv_dispatch_rmpp_rcv: ERR 6506: "
			"Out Of Memory - could not allocate %d bytes for the MADW\n",
			size);

		status = IB_INSUFFICIENT_MEMORY;
		goto dispatch_rmpp_rcv_done;
	}

	p_req_madw = osmv_txn_get_madw(p_txn);
	p_mad_buf = osm_madw_get_mad_ptr(p_new_madw);
	status = osmv_rmpp_recv_ctx_reassemble_arbt_mad(p_recv_ctx, size,
							(uint8_t *) p_mad_buf);
	if (IB_SUCCESS != status) {
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
			"__osmv_dispatch_rmpp_rcv: ERR 6507: "
			"Internal error - could not reassemble the result MAD\n");
		goto dispatch_rmpp_rcv_done;	/* What can happen here? */
	}

	/* The MAD is assembled, we are about to apply the callback.
	 * Delete the transaction context, unless the transaction is double sided */
	if (FALSE == osmv_txn_is_rmpp_init_by_peer(p_txn)
	    || FALSE == osmv_mad_is_multi_resp(p_mad)) {

		osmv_txn_done(h_bind, key, FALSE);
	}

	osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
		"RMPP MADW %p assembly complete, TID=0x%" PRIx64 "\n", p_new_madw,
		tid);

	p_mad_buf->trans_id = cl_hton64(tid);
	osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
		"Restoring the original TID to 0x%" PRIx64 "\n",
		cl_ntoh64(p_mad_buf->trans_id));

	/* Finally, do the job! */
	p_bo->recv_cb(p_new_madw, p_bo->cb_context, p_req_madw);

dispatch_rmpp_rcv_done:
	OSM_LOG_EXIT(p_bo->p_vendor->p_log);
	return status;
}