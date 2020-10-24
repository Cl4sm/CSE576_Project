ib_api_status_t
osmv_rmpp_sar_get_mad_seg(IN osmv_rmpp_sar_t * p_sar,
			  IN uint32_t seg_idx, OUT void *p_buf)
{
	void *p_seg;
	uint32_t sz_left;
	uint32_t num_segs;

	CL_ASSERT(p_sar);

	num_segs = p_sar->data_len / p_sar->data_sz;
	if ((p_sar->data_len % p_sar->data_sz) > 0) {
		num_segs++;
	}

	if ((seg_idx > num_segs) && (seg_idx != 1)) {
		return IB_NOT_FOUND;
	}

	/* cleanup */
	memset(p_buf, 0, MAD_BLOCK_SIZE);

	/* attach header */
	memcpy(p_buf, p_sar->p_arbt_mad, p_sar->hdr_sz);

	/* fill data */
	p_seg =
	    (char *)p_sar->p_arbt_mad + p_sar->hdr_sz +
	    ((seg_idx - 1) * p_sar->data_sz);
	sz_left = p_sar->data_len - ((seg_idx - 1) * p_sar->data_sz);
	if (sz_left > p_sar->data_sz)
		memcpy((char *)p_buf + p_sar->hdr_sz, (char *)p_seg,
		       p_sar->data_sz);
	else
		memcpy((char *)p_buf + p_sar->hdr_sz, (char *)p_seg, sz_left);

	return IB_SUCCESS;
}