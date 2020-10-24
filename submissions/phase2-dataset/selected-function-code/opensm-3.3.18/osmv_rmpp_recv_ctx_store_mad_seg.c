ib_api_status_t
osmv_rmpp_recv_ctx_store_mad_seg(IN osmv_rmpp_recv_ctx_t * p_recv_ctx,
				 IN void *p_mad)
{
	cl_list_obj_t *p_obj = NULL;
	void *p_list_mad;

	OSM_LOG_ENTER(p_recv_ctx->p_log);

	CL_ASSERT(p_recv_ctx);
	p_list_mad = malloc(MAD_BLOCK_SIZE);
	if (NULL == p_list_mad) {
		return IB_INSUFFICIENT_MEMORY;
	}
	memset(p_list_mad, 0, MAD_BLOCK_SIZE);
	memcpy(p_list_mad, p_mad, MAD_BLOCK_SIZE);

	p_obj = malloc(sizeof(cl_list_obj_t));
	if (NULL == p_obj) {
		free(p_list_mad);
		return IB_INSUFFICIENT_MEMORY;
	}
	memset(p_obj, 0, sizeof(cl_list_obj_t));
	cl_qlist_set_obj(p_obj, p_list_mad);

	cl_qlist_insert_tail(p_recv_ctx->p_rbuf, &p_obj->list_item);

	if (osmv_mad_is_sa((ib_mad_t *) p_mad)) {
		p_recv_ctx->is_sa_mad = TRUE;
	}

	return IB_SUCCESS;

}