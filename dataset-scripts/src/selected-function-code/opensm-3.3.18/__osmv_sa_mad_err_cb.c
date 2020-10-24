static void __osmv_sa_mad_err_cb(IN void *bind_context, IN osm_madw_t * p_madw)
{
	osmv_sa_bind_info_t *p_bind = (osmv_sa_bind_info_t *) bind_context;
	osmv_query_req_t *p_query_req_copy = NULL;
	osmv_query_res_t query_res;

	OSM_LOG_ENTER(p_bind->p_log);

	/* Obtain the sent context etc */
	p_query_req_copy =
	    (osmv_query_req_t *) (p_madw->context.arb_context.context1);

	/* provide the context of the original request in the result */
	query_res.query_context = p_query_req_copy->query_context;

	query_res.p_result_madw = p_madw;

	query_res.status = IB_TIMEOUT;
	query_res.result_cnt = 0;
	query_res.p_result_madw->status = IB_TIMEOUT;
	p_madw->status = IB_TIMEOUT;
	query_res.query_type = p_query_req_copy->query_type;

	p_query_req_copy->pfn_query_cb(&query_res);

	if ((p_query_req_copy->flags & OSM_SA_FLAGS_SYNC) == OSM_SA_FLAGS_SYNC)
		cl_event_signal(&p_bind->sync_event);

	free(p_query_req_copy);
	OSM_LOG_EXIT(p_bind->p_log);
}