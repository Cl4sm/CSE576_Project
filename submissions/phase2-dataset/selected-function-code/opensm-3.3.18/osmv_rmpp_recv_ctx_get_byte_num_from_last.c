uint32_t
osmv_rmpp_recv_ctx_get_byte_num_from_last(IN osmv_rmpp_recv_ctx_t * p_recv_ctx)
{
	cl_list_item_t *p_item;
	cl_list_obj_t *p_obj;
	void *p_list_mad;
	uint32_t num_bytes, num_segs;

	p_item = cl_qlist_tail(p_recv_ctx->p_rbuf);
	p_obj = PARENT_STRUCT(p_item, cl_list_obj_t, list_item);
	p_list_mad = cl_qlist_obj(p_obj);

	/* mad data sz */
	num_segs = cl_qlist_count(p_recv_ctx->p_rbuf);
	num_bytes = cl_ntoh32(((ib_rmpp_mad_t *) p_list_mad)->paylen_newwin);

	if (0 != num_bytes) {
		if (p_recv_ctx->is_sa_mad) {
			/* sa mad hdr sz */
			num_bytes += MAD_RMPP_HDR_SIZE;
			num_bytes += (num_segs - 1) * IB_SA_DATA_SIZE;
		} else {
			/* mad hdr sz */
			num_bytes += MAD_RMPP_HDR_SIZE;
			num_bytes += (num_segs - 1) * MAD_RMPP_DATA_SIZE;
		}
	}

	return num_bytes;
}