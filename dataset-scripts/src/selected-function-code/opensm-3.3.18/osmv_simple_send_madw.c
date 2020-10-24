ib_api_status_t
osmv_simple_send_madw(IN osm_bind_handle_t h_bind,
		      IN osm_madw_t * const p_madw,
		      IN osmv_txn_ctx_t * p_txn, IN boolean_t is_retry)
{
	ib_api_status_t ret;
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	osm_mad_addr_t *p_mad_addr = osm_madw_get_mad_addr_ptr(p_madw);
	uint8_t mad_buf[MAD_BLOCK_SIZE];
	ib_mad_t *p_mad = (ib_mad_t *) mad_buf;
	uint64_t key = 0;

	OSM_LOG_ENTER(p_bo->p_vendor->p_log);

	CL_ASSERT(p_madw->mad_size <= MAD_BLOCK_SIZE);

	memset(p_mad, 0, MAD_BLOCK_SIZE);
	memcpy(p_mad, osm_madw_get_mad_ptr(p_madw), p_madw->mad_size);

	if (NULL != p_txn) {
		/* Push a fake txn id to the MAD */
		key = osmv_txn_get_key(p_txn);
		p_mad->trans_id = cl_hton64(key);
	}

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
			    osmv_transport_mad_send(h_bind, p_mad, p_mad_addr);
		}
	} else {
		ret = osmv_transport_mad_send(h_bind, p_mad, p_mad_addr);
	}

	if ((IB_SUCCESS == ret) && (NULL != p_txn) && (!is_retry)) {
		/* Set the timeout for receiving the response MAD */
		ret = osmv_txn_set_timeout_ev(h_bind, key,
					      p_bo->p_vendor->resp_timeout);
	}

	OSM_LOG_EXIT(p_bo->p_vendor->p_log);
	return ret;
}