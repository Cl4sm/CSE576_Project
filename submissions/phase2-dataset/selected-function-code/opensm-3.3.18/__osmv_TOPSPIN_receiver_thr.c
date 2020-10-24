void __osmv_TOPSPIN_receiver_thr(void *p_ctx)
{
	int ts_ret_code;
	struct ib_mad mad;
	osm_mad_addr_t mad_addr;
	osmv_bind_obj_t *const p_bo = (osmv_bind_obj_t *) p_ctx;
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(p_bo->p_vendor->p_log);

	/* Make sure the p_bo object is still relevant */
	if ((p_bo->magic_ptr != p_bo) || p_bo->is_closing)
		return;

	/* we set the type of cancelation for this thread */
	/* pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); */

	while (1) {
		/* Make sure the p_bo object is still relevant */
		if ((p_bo->magic_ptr != p_bo) || p_bo->is_closing)
			return;

		/* we read one mad at a time and pass it to the read callback function */
		ts_ret_code =
		    read(((osmv_TOPSPIN_transport_mgr_t *) (p_bo->
							    p_transp_mgr))->
			 device_fd, &mad, sizeof(mad));
		/* Make sure the p_bo object is still relevant */
		if ((p_bo->magic_ptr != p_bo) || p_bo->is_closing)
			return;

		if (ts_ret_code != sizeof(mad)) {
			osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
				"__osmv_TOPSPIN_receiver_thr: ERR 6803: "
				"error with read, bytes = %d, errno = %d\n",
				ts_ret_code, errno);
			break;
		} else {
			osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
				"__osmv_TOPSPIN_receiver_thr: "
				"MAD QPN:%d SLID:0x%04x class:0x%02x "
				"method:0x%02x attr:0x%04x status:0x%04x "
				"tid:0x%016" PRIx64 "\n",
				mad.dqpn,
				cl_ntoh16(mad.slid),
				mad.mgmt_class,
				mad.r_method,
				cl_ntoh16(mad.attribute_id),
				cl_ntoh16(mad.status),
				cl_ntoh64(mad.transaction_id));

			/* first arrange an address */
			__osmv_TOPSPIN_mad_addr_to_osm_addr(p_bo->p_vendor,
							    &mad,
							    (((ib_mad_t *) &
							      mad)->
							     mgmt_class ==
							     IB_MCLASS_SUBN_LID)
							    ||
							    (((ib_mad_t *) &
							      mad)->
							     mgmt_class ==
							     IB_MCLASS_SUBN_DIR),
							    &mad_addr);

			/* call the receiver callback */

			status =
			    osmv_dispatch_mad((osm_bind_handle_t) p_bo,
					      (void *)&mad, &mad_addr);

			/* Make sure the p_bo object is still relevant */
			if ((p_bo->magic_ptr != p_bo) || p_bo->is_closing)
				return;

			if (IB_INTERRUPTED == status) {

				osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
					"__osmv_TOPSPIN_receiver_thr: "
					"The bind handle %p is being closed. "
					"Breaking the loop.\n", p_bo);
				break;
			}
		}
	}

	OSM_LOG_EXIT(p_bo->p_vendor->p_log);
}