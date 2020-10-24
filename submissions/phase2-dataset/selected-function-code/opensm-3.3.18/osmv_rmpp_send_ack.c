ib_api_status_t
osmv_rmpp_send_ack(IN osm_bind_handle_t h_bind,
		   IN const ib_mad_t * p_req_mad,
		   IN uint32_t seg_num,
		   IN uint32_t nwl, IN const osm_mad_addr_t * p_mad_addr)
{
	uint8_t resp_mad[MAD_BLOCK_SIZE];
	ib_rmpp_mad_t *p_resp_mad = (ib_rmpp_mad_t *) resp_mad;

#ifdef OSMV_RANDOM_DROP
	if (TRUE == osmv_random_drop()) {
		osm_log(((osmv_bind_obj_t *) h_bind)->p_vendor->p_log,
			OSM_LOG_DEBUG,
			"Error injection - dropping the RMPP ACK\n");
		return IB_SUCCESS;
	}
#endif

	memcpy(p_resp_mad, p_req_mad, MAD_BLOCK_SIZE);

	p_resp_mad->common_hdr.method = osmv_invert_method(p_req_mad->method);
	p_resp_mad->rmpp_type = IB_RMPP_TYPE_ACK;
	p_resp_mad->seg_num = cl_hton32(seg_num);
	p_resp_mad->paylen_newwin = cl_hton32(nwl);
	p_resp_mad->rmpp_flags = IB_RMPP_FLAG_ACTIVE;

	return osmv_transport_mad_send(h_bind, p_resp_mad, p_mad_addr);
}