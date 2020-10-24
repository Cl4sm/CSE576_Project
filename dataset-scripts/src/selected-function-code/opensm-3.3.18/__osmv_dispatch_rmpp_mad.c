static void
__osmv_dispatch_rmpp_mad(IN osm_bind_handle_t h_bind,
			 IN const ib_mad_t * p_mad,
			 IN osmv_txn_ctx_t * p_txn,
			 IN const osm_mad_addr_t * p_mad_addr)
{
	ib_api_status_t status = IB_SUCCESS;
	uint64_t key = cl_ntoh64(p_mad->trans_id);
	boolean_t is_init_by_peer = FALSE;
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	osm_madw_t *p_madw;

	OSM_LOG_ENTER(p_bo->p_vendor->p_log);

	if (NULL == p_txn) {
		if (FALSE == osmv_rmpp_is_data(p_mad)
		    || FALSE == osmv_rmpp_is_first(p_mad)) {
			osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
				"The MAD does not match any transaction "
				"and does not start a sender-initiated RMPP transfer.\n");
			goto dispatch_rmpp_mad_done;
		}

		/* IB Spec 13.6.2.2. This is a Sender Initiated Transfer.
		   My peer is the requester and RMPP Sender. I am the RMPP Receiver.
		 */
		status = osmv_txn_init(h_bind, /*tid==key */ key, key, &p_txn);
		if (IB_SUCCESS != status) {
			goto dispatch_rmpp_mad_done;
		}

		is_init_by_peer = TRUE;
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"A new sender-initiated transfer (TID=0x%" PRIx64 ") started\n",
			key);
	}

	if (OSMV_TXN_RMPP_NONE == osmv_txn_get_rmpp_state(p_txn)) {
		/* Case 1: Fall through from above.
		 * Case 2: When the transaction was initiated by me
		 *         (a single request MAD), there was an uncertainty
		 *         whether the reply will be RMPP. Now it's resolved,
		 *         since the reply is RMPP!
		 */
		status =
		    osmv_txn_init_rmpp_receiver(h_bind, p_txn, is_init_by_peer);
		if (IB_SUCCESS != status) {
			goto dispatch_rmpp_mad_done;
		}
	}

	switch (osmv_txn_get_rmpp_state(p_txn)) {

	case OSMV_TXN_RMPP_RECEIVER:
		status =
		    __osmv_dispatch_rmpp_rcv(h_bind, p_mad, p_txn, p_mad_addr);
		if (IB_SUCCESS != status) {
			if (FALSE == osmv_txn_is_rmpp_init_by_peer(p_txn)) {
				/* This is a requester, still waiting for the reply. Apply the callback */
				/* update the status of the p_madw */
				p_madw = osmv_txn_get_madw(p_txn);
				p_madw->status = status;
				p_bo->send_err_cb(p_bo->cb_context, p_madw);
			}

			/* ABORT/STOP/LOCAL ERROR */
			osmv_txn_done(h_bind, osmv_txn_get_key(p_txn), FALSE);
		}
		break;

	case OSMV_TXN_RMPP_SENDER:
		__osmv_dispatch_rmpp_snd(h_bind, p_mad, p_txn, p_mad_addr);
		/* If an error happens here, it's the sender thread to cleanup the txn */
		break;

	default:
		CL_ASSERT(FALSE);
	}

dispatch_rmpp_mad_done:
	OSM_LOG_EXIT(p_bo->p_vendor->p_log);
}