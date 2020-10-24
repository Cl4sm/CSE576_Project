ib_api_status_t
osmv_rmpp_send_ctx_init(osmv_rmpp_send_ctx_t * p_ctx, void *p_arbt_mad,
			uint32_t mad_sz, osm_log_t * p_log)
{
	ib_api_status_t st = IB_SUCCESS;
	cl_status_t cl_st;

	CL_ASSERT(p_ctx);
	if (NULL == p_arbt_mad) {
		return IB_INVALID_PARAMETER;
	}

	if (osmv_mad_is_sa((ib_mad_t *) p_arbt_mad)) {
		p_ctx->is_sa_mad = TRUE;
	} else
		p_ctx->is_sa_mad = FALSE;

	p_ctx->mad_sz = mad_sz;

	cl_event_construct(&p_ctx->event);
	cl_st = cl_event_init(&p_ctx->event, FALSE);
	if (cl_st != CL_SUCCESS) {
		return IB_ERROR;
	}

	st = osmv_rmpp_sar_init(&p_ctx->sar, p_arbt_mad, p_ctx->mad_sz,
				p_ctx->is_sa_mad);
	if (st == IB_SUCCESS) {
		p_ctx->window_first = 1;
		p_ctx->window_last = 1;
	}

	p_ctx->p_log = p_log;
	return st;
}