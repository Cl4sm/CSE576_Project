ib_api_status_t
osmv_rmpp_recv_ctx_reassemble_arbt_mad(IN osmv_rmpp_recv_ctx_t * p_recv_ctx,
				       IN uint32_t size, IN void *p_arbt_mad)
{
	ib_api_status_t st = IB_SUCCESS;

	CL_ASSERT(p_recv_ctx);

	st = osmv_rmpp_sar_init(&p_recv_ctx->sar, p_arbt_mad, size,
				p_recv_ctx->is_sa_mad);
	if (st != IB_SUCCESS) {
		return st;
	}

	st = osmv_rmpp_sar_reassemble_arbt_mad(&p_recv_ctx->sar,
					       p_recv_ctx->p_rbuf);

	osmv_rmpp_sar_done(&p_recv_ctx->sar);

	return st;
}