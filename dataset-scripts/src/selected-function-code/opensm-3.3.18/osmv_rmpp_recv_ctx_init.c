ib_api_status_t
osmv_rmpp_recv_ctx_init(osmv_rmpp_recv_ctx_t * p_ctx, osm_log_t * p_log)
{
	ib_api_status_t st = IB_SUCCESS;

	CL_ASSERT(p_ctx);

	p_ctx->is_sa_mad = FALSE;

	p_ctx->p_rbuf = malloc(sizeof(cl_qlist_t));
	if (p_ctx->p_rbuf) {
		memset(p_ctx->p_rbuf, 0, sizeof(cl_qlist_t));
		cl_qlist_init(p_ctx->p_rbuf);
		p_ctx->expected_seg = 1;
	} else
		st = IB_INSUFFICIENT_MEMORY;

	p_ctx->p_log = p_log;

	return st;
}