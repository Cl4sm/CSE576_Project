void osm_infr_rcv_process(IN void *context, IN void *data)
{
	osm_sa_t *sa = context;
	osm_madw_t *p_madw = data;
	ib_sa_mad_t *p_sa_mad;

	OSM_LOG_ENTER(sa->p_log);

	CL_ASSERT(p_madw);

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);

	CL_ASSERT(p_sa_mad->attr_id == IB_MAD_ATTR_INFORM_INFO);

	if (p_sa_mad->method != IB_MAD_METHOD_SET) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Unsupported Method (%s) for InformInfo\n",
			ib_get_sa_method_str(p_sa_mad->method));
		osm_sa_send_error(sa, p_madw, IB_MAD_STATUS_UNSUP_METHOD_ATTR);
		goto Exit;
	}

	infr_rcv_process_set_method(sa, p_madw);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}