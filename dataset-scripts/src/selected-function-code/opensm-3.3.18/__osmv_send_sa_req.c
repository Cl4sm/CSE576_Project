static ib_api_status_t
__osmv_send_sa_req(IN osmv_sa_bind_info_t * p_bind,
		   IN const osmv_sa_mad_data_t * const p_sa_mad_data,
		   IN const osmv_query_req_t * const p_query_req)
{
	ib_api_status_t status;
	ib_mad_t *p_mad_hdr;
	ib_sa_mad_t *p_sa_mad;
	osm_madw_t *p_madw;
	osm_log_t *p_log = p_bind->p_log;
	static atomic32_t trans_id;
	boolean_t sync;
	osmv_query_req_t *p_query_req_copy;
	uint32_t sa_size;

	OSM_LOG_ENTER(p_log);

	/*
	   since the sm_lid might change we obtain it every send
	   (actually it is cached in the bind object and refreshed
	   every 30sec by this proc)
	 */
	status =
	    __osmv_get_lid_and_sm_lid_by_port_guid(p_bind->p_vendor,
						   p_bind->port_guid,
						   &p_bind->
						   last_lids_update_sec,
						   &p_bind->lid,
						   &p_bind->sm_lid);
	if (status != IB_SUCCESS) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0509: "
			"Failed to obtain the SM lid\n");
		goto Exit;
	}

	/* Get a MAD wrapper for the send */
	p_madw = osm_mad_pool_get(p_bind->p_mad_pool,
				  p_bind->h_bind, MAD_BLOCK_SIZE, NULL);

	if (p_madw == NULL) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0510: "
			"Unable to acquire MAD\n");
		status = IB_INSUFFICIENT_RESOURCES;
		goto Exit;
	}

	/* Initialize the Sent MAD: */

	/* Initialize the MAD buffer for the send operation. */
	p_mad_hdr = osm_madw_get_mad_ptr(p_madw);
	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);

	/* Get a new transaction Id */
	cl_atomic_inc(&trans_id);

	/* Cleanup the MAD from any residue */
	memset(p_sa_mad, 0, MAD_BLOCK_SIZE);

	/* Initialize the standard MAD header. */
	ib_mad_init_new(p_mad_hdr,	/* mad pointer */
			IB_MCLASS_SUBN_ADM,	/* class */
			(uint8_t) 2,	/* version */
			p_sa_mad_data->method,	/* method */
			cl_hton64((uint64_t) trans_id),	/* tid */
			p_sa_mad_data->attr_id,	/* attr id */
			p_sa_mad_data->attr_mod	/* attr mod */);

	/* Set the query information. */
	p_sa_mad->sm_key = p_query_req->sm_key;
	p_sa_mad->attr_offset = 0;
	p_sa_mad->comp_mask = p_sa_mad_data->comp_mask;
	if (p_sa_mad->comp_mask) {
		p_sa_mad_data->attr_offset ? (sa_size = ib_get_attr_size(p_sa_mad_data->attr_offset)) : (sa_size = IB_SA_DATA_SIZE);
		memcpy(p_sa_mad->data, p_sa_mad_data->p_attr, sa_size);
	}

	/*
	   Provide the address to send to
	 */
	/* Patch to handle IBAL - host order , where it should take destination lid in network order */
#ifdef OSM_VENDOR_INTF_AL
	p_madw->mad_addr.dest_lid = p_bind->sm_lid;
#else
	p_madw->mad_addr.dest_lid = cl_hton16(p_bind->sm_lid);
#endif
	p_madw->mad_addr.addr_type.smi.source_lid = cl_hton16(p_bind->lid);
	p_madw->mad_addr.addr_type.gsi.remote_qp = CL_HTON32(1);
	p_madw->resp_expected = TRUE;
	p_madw->fail_msg = CL_DISP_MSGID_NONE;

	/*
	   Provide MAD context such that the call back will know what to do.
	   We have to keep the entire request structure so we know the CB.
	   Since we can not rely on the client to keep it around until
	   the response - we duplicate it and will later dispose it (in CB).
	   To store on the MADW we cast it into what opensm has:
	   p_madw->context.arb_context.context1
	 */
	p_query_req_copy = malloc(sizeof(*p_query_req_copy));
	if (!p_query_req_copy) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0511: "
			"Unable to acquire memory for query copy\n");
		osm_mad_pool_put(p_bind->p_mad_pool, p_madw);
		status = IB_INSUFFICIENT_RESOURCES;
		goto Exit;
	}
	*p_query_req_copy = *p_query_req;
	p_madw->context.arb_context.context1 = p_query_req_copy;

	/* we can support async as well as sync calls */
	sync = ((p_query_req->flags & OSM_SA_FLAGS_SYNC) == OSM_SA_FLAGS_SYNC);

	/* send the mad asynchronously */
	status = osm_vendor_send(osm_madw_get_bind_handle(p_madw),
				 p_madw, p_madw->resp_expected);

	/* if synchronous - wait on the event */
	if (sync) {
		OSM_LOG(p_log, OSM_LOG_DEBUG, "Waiting for async event\n");
		cl_event_wait_on(&p_bind->sync_event, EVENT_NO_TIMEOUT, FALSE);
		cl_event_reset(&p_bind->sync_event);
		status = p_madw->status;
	}

Exit:
	OSM_LOG_EXIT(p_log);
	return status;
}