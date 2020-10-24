void __osmv_ibms_receiver_callback(void *p_ctx, ibms_mad_msg_t * p_mad)
{
	osm_mad_addr_t mad_addr;
	osmv_bind_obj_t *const p_bo = (osmv_bind_obj_t *) p_ctx;
	ib_api_status_t status = IB_SUCCESS;

	/* Make sure the p_bo object is still relevant */
	if ((p_bo->magic_ptr != p_bo) || p_bo->is_closing)
		return;

	{
		OSM_LOG_ENTER(p_bo->p_vendor->p_log);

		/* some logging */
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"__osmv_ibms_receiver_callback: "
			"MAD QPN:%d SLID:0x%04x class:0x%02x "
			"method:0x%02x attr:0x%04x status:0x%04x "
			"tid:0x%016" PRIx64 "\n",
			p_mad->addr.dqpn,
			cl_ntoh16(p_mad->addr.slid),
			p_mad->header.mgmt_class,
			p_mad->header.method,
			cl_ntoh16(p_mad->header.attr_id),
			cl_ntoh16(p_mad->header.status),
			cl_ntoh64(p_mad->header.trans_id));

		/* first arrange an address */
		__osmv_ibms_mad_addr_to_osm_addr(p_bo->p_vendor,
						 &p_mad->addr,
						 (((ib_mad_t *) & p_mad->
						   header)->mgmt_class ==
						  IB_MCLASS_SUBN_LID)
						 ||
						 (((ib_mad_t *) & p_mad->
						   header)->mgmt_class ==
						  IB_MCLASS_SUBN_DIR),
						 &mad_addr);

		/* call the receiver callback */

		status =
		    osmv_dispatch_mad((osm_bind_handle_t) p_bo,
				      (void *)&p_mad->header, &mad_addr);

		OSM_LOG_EXIT(p_bo->p_vendor->p_log);
	}
}