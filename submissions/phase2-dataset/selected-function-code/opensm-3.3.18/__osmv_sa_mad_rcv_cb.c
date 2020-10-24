static void
__osmv_sa_mad_rcv_cb(IN osm_madw_t * p_madw,
		     IN void *bind_context, IN osm_madw_t * p_req_madw)
{
	osmv_sa_bind_info_t *p_bind = (osmv_sa_bind_info_t *) bind_context;
	osmv_query_req_t *p_query_req_copy = NULL;
	osmv_query_res_t query_res;
	ib_sa_mad_t *p_sa_mad;
	ib_net16_t mad_status;

	OSM_LOG_ENTER(p_bind->p_log);

	if (!p_req_madw) {
		OSM_LOG(p_bind->p_log, OSM_LOG_DEBUG,
			"Ignoring a non-response mad\n");
		osm_mad_pool_put(p_bind->p_mad_pool, p_madw);
		goto Exit;
	}

	/* obtain the sent context */
	p_query_req_copy =
	    (osmv_query_req_t *) (p_req_madw->context.arb_context.context1);

	/* provide the context of the original request in the result */
	query_res.query_context = p_query_req_copy->query_context;

	/* provide the resulting madw */
	query_res.p_result_madw = p_madw;

	/* update the req fields */
	p_sa_mad = (ib_sa_mad_t *) p_madw->p_mad;

	/* if we got a remote error track it in the status */
	mad_status = (ib_net16_t) (p_sa_mad->status & IB_SMP_STATUS_MASK);
	if (mad_status != IB_SUCCESS) {
		OSM_LOG(p_bind->p_log, OSM_LOG_ERROR, "ERR 0501: "
			"Remote error:0x%04X\n", cl_ntoh16(mad_status));
		query_res.status = IB_REMOTE_ERROR;
	} else
		query_res.status = IB_SUCCESS;

	/* what if we have got back an empty mad ? */
	if (!p_madw->mad_size) {
		OSM_LOG(p_bind->p_log, OSM_LOG_ERROR, "ERR 0502: "
			"Got an empty mad\n");
		query_res.status = IB_ERROR;
	}

	if (IB_SUCCESS == mad_status) {

		/* if we are in not in a method response of an rmpp nature we must get only 1 */
		/* HACK: in the future we might need to be smarter for other methods... */
		if (p_sa_mad->method != IB_MAD_METHOD_GETTABLE_RESP) {
			query_res.result_cnt = 1;
		} else {
#ifndef VENDOR_RMPP_SUPPORT
			if (mad_status != IB_SUCCESS)
				query_res.result_cnt = 0;
			else
				query_res.result_cnt = 1;
#else
			/* we used the offset value to calculate the number of
			   records in here */
			if (ib_get_attr_size(p_sa_mad->attr_offset) == 0) {
				query_res.result_cnt = 0;
				OSM_LOG(p_bind->p_log, OSM_LOG_DEBUG,
					"Count = 0\n");
			}
			else {
				query_res.result_cnt =
					(p_madw->mad_size - IB_SA_MAD_HDR_SIZE) /
					ib_get_attr_size(p_sa_mad->attr_offset);
				OSM_LOG(p_bind->p_log, OSM_LOG_DEBUG,
					"Count = %u = %zu / %u (%zu)\n",
					query_res.result_cnt,
					p_madw->mad_size - IB_SA_MAD_HDR_SIZE,
					ib_get_attr_size(p_sa_mad->attr_offset),
					(p_madw->mad_size - IB_SA_MAD_HDR_SIZE) %
					ib_get_attr_size(p_sa_mad->attr_offset));
			}
#endif
		}
	}

	query_res.query_type = p_query_req_copy->query_type;

	p_query_req_copy->pfn_query_cb(&query_res);

	if ((p_query_req_copy->flags & OSM_SA_FLAGS_SYNC) == OSM_SA_FLAGS_SYNC)
		cl_event_signal(&p_bind->sync_event);

Exit:

	/* free the copied query request if found */
	if (p_query_req_copy)
		free(p_query_req_copy);

	/* put back the request madw */
	if (p_req_madw)
		osm_mad_pool_put(p_bind->p_mad_pool, p_req_madw);

	OSM_LOG_EXIT(p_bind->p_log);
}