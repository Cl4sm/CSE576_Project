void osmv_rmpp_recv_ctx_done(IN osmv_rmpp_recv_ctx_t * p_ctx)
{
	cl_list_item_t *p_list_item;
	cl_list_obj_t *p_obj;

	CL_ASSERT(p_ctx);

	/* go over all the items in the list and remove them */
	p_list_item = cl_qlist_remove_head(p_ctx->p_rbuf);
	while (p_list_item != cl_qlist_end(p_ctx->p_rbuf)) {

		p_obj = PARENT_STRUCT(p_list_item, cl_list_obj_t, list_item);

		free(cl_qlist_obj(p_obj));
		free(p_obj);

		p_list_item = cl_qlist_remove_head(p_ctx->p_rbuf);
	}

	osmv_rmpp_sar_done(&p_ctx->sar);

	free(p_ctx->p_rbuf);
	free(p_ctx);
}