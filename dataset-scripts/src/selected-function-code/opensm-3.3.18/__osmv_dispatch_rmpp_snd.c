static void
__osmv_dispatch_rmpp_snd(IN osm_bind_handle_t h_bind,
			 IN const ib_mad_t * p_mad,
			 IN osmv_txn_ctx_t * p_txn,
			 IN const osm_mad_addr_t * p_mad_addr)
{
	osmv_rmpp_send_ctx_t *p_send_ctx = osmv_txn_get_rmpp_send_ctx(p_txn);

	uint32_t old_wl = p_send_ctx->window_last;
	uint32_t total_segs = osmv_rmpp_send_ctx_get_num_segs(p_send_ctx);
	uint32_t seg_num = cl_ntoh32(((ib_rmpp_mad_t *) p_mad)->seg_num);
	uint32_t new_wl = cl_ntoh32(((ib_rmpp_mad_t *) p_mad)->paylen_newwin);
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;

	OSM_LOG_ENTER(p_bo->p_vendor->p_log);

	if (TRUE == osmv_rmpp_is_abort_stop(p_mad)) {

		osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
			"__osmv_dispatch_rmpp_snd: ERR 6502: "
			"The remote side sent an ABORT/STOP indication.\n");
		osmv_rmpp_snd_error(p_send_ctx, IB_REMOTE_ERROR);
		goto dispatch_rmpp_snd_done;
	}

	if (FALSE == osmv_rmpp_is_ack(p_mad)) {

		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"Not supposed to receive DATA packets --> dropping the MAD\n");
		goto dispatch_rmpp_snd_done;
	}

	/* Continue processing the ACK */
	if (seg_num > old_wl) {

		osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
			"__osmv_dispatch_rmpp_snd: ERR 6503: "
			"ACK received for a non-sent segment %d\n", seg_num);

		osmv_rmpp_send_nak(h_bind, p_mad, p_mad_addr,
				   IB_RMPP_TYPE_ABORT, IB_RMPP_STATUS_S2B);

		osmv_rmpp_snd_error(p_send_ctx, IB_REMOTE_ERROR);
		goto dispatch_rmpp_snd_done;
	}

	osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
		"__osmv_dispatch_rmpp_snd: "
		"New WL = %u Old WL = %u Total Segs = %u\n",
		new_wl, old_wl, total_segs);

	if (new_wl < old_wl) {
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
			"__osmv_dispatch_rmpp_snd: ERR 6508: "
			"The receiver requests a smaller WL (%d) than before (%d)\n",
			new_wl, old_wl);

		osmv_rmpp_send_nak(h_bind, p_mad, p_mad_addr,
				   IB_RMPP_TYPE_ABORT, IB_RMPP_STATUS_W2S);

		osmv_rmpp_snd_error(p_send_ctx, IB_REMOTE_ERROR);
		goto dispatch_rmpp_snd_done;
	}

	/* Update the sender's window, and optionally wake up the sender thread
	 * Note! A single ACK can acknowledge a whole range of segments: [WF..SEG_NUM]
	 */
	osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
		"ACK for seg_num #%d accepted.\n", seg_num);

	if (seg_num == old_wl) {

		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"The send window [%d:%d] is totally acknowledged.\n",
			p_send_ctx->window_first, old_wl);

		p_send_ctx->window_first = seg_num + 1;
		p_send_ctx->window_last =
		    (new_wl < total_segs) ? new_wl : total_segs;

		/* Remove the response timeout event for the window */
		osmv_txn_remove_timeout_ev(h_bind, osmv_txn_get_key(p_txn));

		/* Wake up the sending thread */
		cl_event_signal(&p_send_ctx->event);
	}

dispatch_rmpp_snd_done:
	OSM_LOG_EXIT(p_bo->p_vendor->p_log);
}