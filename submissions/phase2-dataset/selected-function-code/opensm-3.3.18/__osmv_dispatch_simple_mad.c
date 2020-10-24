static void
__osmv_dispatch_simple_mad(IN osm_bind_handle_t h_bind,
			   IN const ib_mad_t * p_mad,
			   IN osmv_txn_ctx_t * p_txn,
			   IN const osm_mad_addr_t * p_mad_addr)
{
	osm_madw_t *p_madw;
	ib_mad_t *p_mad_buf;
	osm_madw_t *p_req_madw = NULL;
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;

	OSM_LOG_ENTER(p_bo->p_vendor->p_log);

	/* Build the MAD wrapper to be returned to the user.
	 * The actual storage for the MAD is allocated there.
	 */
	p_madw =
	    osm_mad_pool_get(p_bo->p_osm_pool, h_bind, MAD_BLOCK_SIZE,
			     p_mad_addr);

	if (NULL == p_madw) {
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
			"__osmv_dispatch_simple_mad: ERR 6501: "
			"Out Of Memory - could not allocate a buffer of size %d\n",
			MAD_BLOCK_SIZE);

		goto dispatch_simple_mad_done;
	}

	p_mad_buf = osm_madw_get_mad_ptr(p_madw);
	/* Copy the payload to the MAD buffer */
	memcpy((void *)p_mad_buf, (void *)p_mad, MAD_BLOCK_SIZE);

	if (NULL != p_txn) {
		/* This is a RESPONSE MAD. Pair it with the REQUEST MAD, pass upstream */
		p_req_madw = p_txn->p_madw;
		CL_ASSERT(NULL != p_req_madw);

		p_mad_buf->trans_id = cl_hton64(osmv_txn_get_tid(p_txn));
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"Restoring the original TID to 0x%" PRIx64 "\n",
			cl_ntoh64(p_mad_buf->trans_id));

		/* Reply matched, transaction complete */
		osmv_txn_done(h_bind, osmv_txn_get_key(p_txn), FALSE);
	} else {
		/* This is a REQUEST  MAD. Don't create a context, pass upstream */
	}

	/* Do the job ! */
	p_bo->recv_cb(p_madw, p_bo->cb_context, p_req_madw);

dispatch_simple_mad_done:
	OSM_LOG_EXIT(p_bo->p_vendor->p_log);
}