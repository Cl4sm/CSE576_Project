static ib_api_status_t
__osmv_rmpp_send_segment(IN osm_bind_handle_t h_bind,
			 IN osmv_txn_ctx_t * p_txn, IN uint32_t seg_num)
{
	ib_api_status_t ret;
	osmv_rmpp_send_ctx_t *p_send_ctx;
	uint8_t mad_buf[MAD_BLOCK_SIZE];
	ib_mad_t *p_mad = (ib_mad_t *) mad_buf;
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	osm_mad_addr_t *p_mad_addr =
	    osm_madw_get_mad_addr_ptr(osmv_txn_get_madw(p_txn));
	uint32_t timeout = p_bo->p_vendor->resp_timeout;
	uint64_t key;

	OSM_LOG_ENTER(p_bo->p_vendor->p_log);

#ifdef OSMV_RANDOM_DROP
	if (TRUE == osmv_random_drop()) {

		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"Error injection - simulating the RMPP segment drop\n");
		return IB_SUCCESS;
	}
#endif

	p_send_ctx = osmv_txn_get_rmpp_send_ctx(p_txn);
	key = osmv_txn_get_key(p_txn);

	if (0 != seg_num) {
		ret =
		    osmv_rmpp_send_ctx_get_seg(p_send_ctx, seg_num, timeout,
					       p_mad);
		CL_ASSERT(IB_SUCCESS == ret);

		/* Put the segment to the wire ! */
		p_mad->trans_id = cl_hton64(key);

		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"Sending RMPP segment #%d, on-wire TID=0x%" PRIx64 "\n",
			seg_num, p_mad->trans_id);

		/*
		   Add call for packet drop randomizer.
		   This is a testing feature. If run_randomizer flag is set to TRUE,
		   the randomizer will be called, and randomally will drop
		   a packet. This is used for simulating unstable fabric.
		 */
		if (p_bo->p_vendor->run_randomizer == TRUE) {
			/* Try the randomizer */
			if (osm_pkt_randomizer_mad_drop(p_bo->p_vendor->p_log,
							p_bo->p_vendor->
							p_pkt_randomizer,
							p_mad) == TRUE) {
				osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
					"The MAD will not be sent. \n");
				ret = IB_SUCCESS;
			} else {
				ret =
				    osmv_transport_mad_send((osm_bind_handle_t)
							    p_bo, p_mad,
							    p_mad_addr);
			}
		} else {
			ret =
			    osmv_transport_mad_send((osm_bind_handle_t) p_bo,
						    p_mad, p_mad_addr);
		}
	} else {
		/* This is an ACK for double-sided handshake. Give it a special treatment. */

		/* It doesn't really matter which data to put. Only the header matters. */
		ret = osmv_rmpp_send_ctx_get_seg(p_send_ctx, 1, timeout, p_mad);
		CL_ASSERT(IB_SUCCESS == ret);

		p_mad->trans_id = cl_hton64(key);
		ret =
		    osmv_rmpp_send_ack((osm_bind_handle_t) p_bo, p_mad,
				       0 /* segnum */ ,
				       OSMV_RMPP_RECV_WIN /* NWL */ ,
				       p_mad_addr);
	}

	OSM_LOG_EXIT(p_bo->p_vendor->p_log);
	return ret;
}