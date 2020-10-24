static ib_api_status_t
__osmv_get_send_txn(IN osm_bind_handle_t h_bind,
		    IN osm_madw_t * const p_madw,
		    IN boolean_t is_rmpp,
		    IN boolean_t resp_expected, OUT osmv_txn_ctx_t ** pp_txn)
{
	ib_api_status_t ret;
	uint64_t tid, key;
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	ib_mad_t *p_mad = osm_madw_get_mad_ptr(p_madw);

	OSM_LOG_ENTER(p_bo->p_vendor->p_log);
	CL_ASSERT(NULL != pp_txn);

	key = tid = cl_ntoh64(p_mad->trans_id);
	if (TRUE == resp_expected) {
		/* Create a unique identifier at the requester side */
		key = osmv_txn_uniq_key(tid);
	}

	/* We must run under a transaction framework */
	ret = osmv_txn_lookup(h_bind, key, pp_txn);
	if (IB_NOT_FOUND == ret) {
		/* Generally, we start a new transaction */
		ret = osmv_txn_init(h_bind, tid, key, pp_txn);
		if (IB_SUCCESS != ret) {
			osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
				"__osmv_get_send_txn: ERR 7313: "
				"The transaction id=0x%" PRIx64 " failed to init.\n",
				tid);
			goto get_send_txn_done;
		}
	} else {
		CL_ASSERT(NULL != *pp_txn);
		/* The transaction context exists.
		 * This is legal only if I am going to return an
		 * (RMPP?) reply to an RMPP request sent by the other part
		 * (double-sided RMPP transfer)
		 */
		if (FALSE == is_rmpp
		    || FALSE == osmv_txn_is_rmpp_init_by_peer(*pp_txn)) {
			osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
				"__osmv_get_send_txn: ERR 7314: "
				"The transaction id=0x%" PRIx64 " is not unique. Send failed.\n",
				tid);

			ret = IB_INVALID_SETTING;
			goto get_send_txn_done;
		}

		if (TRUE == resp_expected) {
			osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
				"__osmv_get_send_txn: ERR 7315: "
				"The transaction id=0x%" PRIx64 " can't expect a response. Send failed.\n",
				tid);

			ret = IB_INVALID_PARAMETER;
			goto get_send_txn_done;
		}
	}

	if (TRUE == is_rmpp) {
		ret = osmv_txn_init_rmpp_sender(h_bind, *pp_txn, p_madw);
		if (IB_SUCCESS != ret) {
			osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
				"__osmv_get_send_txn: ERR 7316: "
				"The transaction id=0x%" PRIx64 " failed to init the rmpp mad. Send failed.\n",
				tid);
			osmv_txn_done(h_bind, tid, FALSE);
			goto get_send_txn_done;
		}
	}

	/* Save a reference to the MAD in the txn context
	 * We'll need to match it in two cases:
	 *  (1) When the response is returned, if I am the requester
	 *  (2) In RMPP retransmissions
	 */
	osmv_txn_set_madw(*pp_txn, p_madw);

get_send_txn_done:
	OSM_LOG_EXIT(p_bo->p_vendor->p_log);

	return ret;
}