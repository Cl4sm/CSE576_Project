ib_api_status_t
osmv_rmpp_send_ctx_get_seg(IN osmv_rmpp_send_ctx_t * p_send_ctx,
			   IN uint32_t seg_idx,
			   IN uint32_t resp_timeout, OUT void *p_buf)
{
	ib_api_status_t st = IB_SUCCESS;
	uint32_t num_segs, paylen = 0;
	ib_rmpp_mad_t *p_rmpp_mad;

	OSM_LOG_ENTER(p_send_ctx->p_log);
	CL_ASSERT(p_send_ctx);

	st = osmv_rmpp_sar_get_mad_seg(&p_send_ctx->sar, seg_idx, p_buf);
	if (st != IB_SUCCESS) {
		goto Exit;
	}

	p_rmpp_mad = (ib_rmpp_mad_t *) p_buf;
	/* Set the relevant bits in the RMPP hdr */
	p_rmpp_mad->rmpp_status = IB_RMPP_STATUS_SUCCESS;
	p_rmpp_mad->rmpp_flags |= IB_RMPP_FLAG_ACTIVE;
	p_rmpp_mad->rmpp_flags |= resp_timeout << 3;

	num_segs = osmv_rmpp_send_ctx_get_num_segs(p_send_ctx);

	if (1 == seg_idx) {
		p_rmpp_mad->rmpp_flags |= IB_RMPP_FLAG_FIRST;

		/* This is the first segment -
		   the reported paylen is the total amount of data.
		 */
		if (p_send_ctx->is_sa_mad) {
			/* sa mad hdr sz */
			paylen = p_send_ctx->mad_sz - IB_SA_MAD_HDR_SIZE;
			paylen +=
			    num_segs * (IB_SA_MAD_HDR_SIZE - MAD_RMPP_HDR_SIZE);
		} else {
			/* mad hdr sz */
			paylen = p_send_ctx->mad_sz - MAD_RMPP_HDR_SIZE;
		}
	}

	if (seg_idx == num_segs) {
		p_rmpp_mad->rmpp_flags |= IB_RMPP_FLAG_LAST;

		/*
		   This is the last segment -
		   the reported paylen is only the amount of data left on this segment.
		 */
		if (p_send_ctx->is_sa_mad) {
			paylen = p_send_ctx->mad_sz - IB_SA_MAD_HDR_SIZE;
			paylen -= (num_segs - 1) * IB_SA_DATA_SIZE;
			paylen += (IB_SA_MAD_HDR_SIZE - MAD_RMPP_HDR_SIZE);
		} else {
			paylen = p_send_ctx->mad_sz - MAD_RMPP_HDR_SIZE;
			paylen -=
			    (num_segs - 1) * (MAD_BLOCK_SIZE -
					      MAD_RMPP_HDR_SIZE);
		}
	}

	p_rmpp_mad->rmpp_type = IB_RMPP_TYPE_DATA;
	p_rmpp_mad->rmpp_version = 1;
	p_rmpp_mad->paylen_newwin = cl_ntoh32(paylen);
	p_rmpp_mad->seg_num = cl_ntoh32(seg_idx);

Exit:
	OSM_LOG_EXIT(p_send_ctx->p_log);
	return st;
}