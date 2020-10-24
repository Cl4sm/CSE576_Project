ib_api_status_t
osmv_transport_mad_send(IN const osm_bind_handle_t h_bind,
			IN void *p_mad, IN const osm_mad_addr_t * p_mad_addr)
{

	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	osm_vendor_t const *p_vend = p_bo->p_vendor;
	int ret;
	ibms_mad_msg_t mad_msg;
	ib_api_status_t status;

	const ib_mad_t *p_mad_hdr = p_mad;

	OSM_LOG_ENTER(p_vend->p_log);

	memset(&mad_msg, 0, sizeof(mad_msg));

	/* Make sure the p_bo object is still relevant */
	if ((p_bo->magic_ptr != p_bo) || p_bo->is_closing)
		return IB_INVALID_CALLBACK;

	/*
	 * Copy the MAD over to the sent mad
	 */
	memcpy(&mad_msg.header, p_mad_hdr, MAD_BLOCK_SIZE);

	/*
	 * For all sends other than directed route SM MADs,
	 * acquire an address vector for the destination.
	 */
	if (p_mad_hdr->mgmt_class != IB_MCLASS_SUBN_DIR) {

		__osmv_ibms_osm_addr_to_mad_addr(p_mad_addr,
						 p_mad_hdr->mgmt_class ==
						 IB_MCLASS_SUBN_LID,
						 &mad_msg.addr);
	} else {
		/* is a directed route - we need to construct a permissive address */
		/* we do not need port number since it is part of the mad_hndl */
		mad_msg.addr.dlid = IB_LID_PERMISSIVE;
		mad_msg.addr.slid = IB_LID_PERMISSIVE;
		mad_msg.addr.sqpn = 0;
		mad_msg.addr.dqpn = 0;
	}

	osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
		"osmv_transport_mad_send: "
		"Sending QPN:%d DLID:0x%04x class:0x%02x "
		"method:0x%02x attr:0x%04x status:0x%04x "
		"tid:0x%016" PRIx64 "\n",
		mad_msg.addr.dqpn,
		cl_ntoh16(mad_msg.addr.dlid),
		mad_msg.header.mgmt_class,
		mad_msg.header.method,
		cl_ntoh16(mad_msg.header.attr_id),
		cl_ntoh16(mad_msg.header.status),
		cl_ntoh64(mad_msg.header.trans_id)
	    );

	/* send it */
	ret =
	    ibms_send(((osmv_ibms_transport_mgr_t *) (p_bo->p_transp_mgr))->
		      conHdl, &mad_msg);
	if (ret) {
		osm_log(p_vend->p_log, OSM_LOG_ERROR,
			"osmv_transport_mad_send: ERR 5304: "
			"Error sending mad (%d).\n", ret);
		status = IB_ERROR;
		goto Exit;
	}

	status = IB_SUCCESS;

Exit:
	OSM_LOG_EXIT(p_vend->p_log);
	return (status);
}