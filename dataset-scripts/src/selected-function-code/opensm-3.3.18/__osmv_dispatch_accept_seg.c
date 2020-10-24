static ib_api_status_t
__osmv_dispatch_accept_seg(IN osm_bind_handle_t h_bind,
			   IN osmv_txn_ctx_t * p_txn, IN const ib_mad_t * p_mad)
{
	ib_api_status_t ret = IB_SUCCESS;
	uint32_t seg_num = cl_ntoh32(((ib_rmpp_mad_t *) p_mad)->seg_num);
	osmv_rmpp_recv_ctx_t *p_recv_ctx = osmv_txn_get_rmpp_recv_ctx(p_txn);
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	uint64_t tid = osmv_txn_get_tid(p_txn);

	OSM_LOG_ENTER(p_bo->p_vendor->p_log);

	if (seg_num != p_recv_ctx->expected_seg) {
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"TID 0x%" PRIx64 ": can't accept this segment (%d) - "
			"this is a Go-Back-N implementation\n", tid, seg_num);
		return IB_INSUFFICIENT_RESOURCES;
	}

	/* Store the packet's copy in the reassembly list.
	 * Promote the expected segment counter.
	 */
	ret = osmv_rmpp_recv_ctx_store_mad_seg(p_recv_ctx, (uint8_t *) p_mad);
	if (IB_SUCCESS != ret) {
		return ret;
	}

	osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
		"TID 0x%" PRIx64 ": segment %d accepted\n", tid, seg_num);
	p_recv_ctx->expected_seg = seg_num + 1;

	OSM_LOG_EXIT(p_bo->p_vendor->p_log);
	return IB_SUCCESS;
}