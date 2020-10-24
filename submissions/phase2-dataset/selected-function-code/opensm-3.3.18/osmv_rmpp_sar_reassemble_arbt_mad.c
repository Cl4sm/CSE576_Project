ib_api_status_t
osmv_rmpp_sar_reassemble_arbt_mad(osmv_rmpp_sar_t * p_sar, cl_qlist_t * p_bufs)
{
	void *buf_tmp, *p_mad;
	cl_list_item_t *p_item;
	cl_list_obj_t *p_obj;
	uint32_t space_left = p_sar->data_len + p_sar->hdr_sz;

	CL_ASSERT(p_sar);
	CL_ASSERT(FALSE == cl_is_qlist_empty(p_bufs));

	/* attach header */
	p_mad = p_sar->p_arbt_mad;
	p_item = cl_qlist_head(p_bufs);
	p_obj = PARENT_STRUCT(p_item, cl_list_obj_t, list_item);
	buf_tmp = cl_qlist_obj(p_obj);
	memcpy(p_mad, buf_tmp, p_sar->hdr_sz);
	p_mad = (char *)p_mad + p_sar->hdr_sz;
	space_left -= p_sar->hdr_sz;

	/* reassemble data */
	while (FALSE == cl_is_qlist_empty(p_bufs)) {

		p_item = cl_qlist_remove_head(p_bufs);
		p_obj = PARENT_STRUCT(p_item, cl_list_obj_t, list_item);
		buf_tmp = cl_qlist_obj(p_obj);

		if (FALSE == cl_is_qlist_empty(p_bufs)) {
			memcpy((char *)p_mad, (char *)buf_tmp + p_sar->hdr_sz,
			       p_sar->data_sz);
			p_mad = (char *)p_mad + p_sar->data_sz;
			space_left -= p_sar->data_sz;
		} else {
			/* the last mad on the list */
			memcpy((char *)p_mad, (char *)buf_tmp + p_sar->hdr_sz,
			       space_left);
			p_mad = (char *)p_mad + space_left;
		}

		free(buf_tmp);
		free(p_obj);
	}

	return IB_SUCCESS;
}