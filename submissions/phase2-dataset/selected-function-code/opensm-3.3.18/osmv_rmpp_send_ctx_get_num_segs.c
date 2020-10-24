uint32_t osmv_rmpp_send_ctx_get_num_segs(IN osmv_rmpp_send_ctx_t * p_send_ctx)
{
	uint32_t data_len, data_sz, num;

	CL_ASSERT(p_send_ctx);

	if (p_send_ctx->is_sa_mad) {
		data_len = p_send_ctx->mad_sz - IB_SA_MAD_HDR_SIZE;
		data_sz = IB_SA_DATA_SIZE;
	} else {
		data_len = p_send_ctx->mad_sz - MAD_RMPP_HDR_SIZE;
		data_sz = MAD_RMPP_DATA_SIZE;
	}

	num = data_len / data_sz;
	if (0 == data_len || (data_len % data_sz) > 0) {
		num++;
	}

	return num;
}