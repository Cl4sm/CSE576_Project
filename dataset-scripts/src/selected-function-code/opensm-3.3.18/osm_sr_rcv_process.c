void osm_sr_rcv_process(IN void *context, IN void *data)
{
	osm_sa_t *sa = context;
	osm_madw_t *p_madw = data;
	ib_sa_mad_t *p_sa_mad;
	boolean_t valid;

	OSM_LOG_ENTER(sa->p_log);

	CL_ASSERT(p_madw);

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);

	CL_ASSERT(p_sa_mad->attr_id == IB_MAD_ATTR_SERVICE_RECORD);

	switch (p_sa_mad->method) {
	case IB_MAD_METHOD_SET:
		cl_plock_excl_acquire(sa->p_lock);
		valid = validate_sr(sa, p_madw);
		if (!valid) {
			cl_plock_release(sa->p_lock);
			OSM_LOG(sa->p_log, OSM_LOG_VERBOSE,
				"Component Mask check failed for set request\n");
			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_REQ_INVALID);
			goto Exit;
		}
		sr_rcv_process_set_method(sa, p_madw);
		break;
	case IB_MAD_METHOD_DELETE:
		cl_plock_excl_acquire(sa->p_lock);
		valid = validate_sr(sa, p_madw);
		if (!valid) {
			cl_plock_release(sa->p_lock);
			OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
				"Component Mask check failed for delete request\n");
			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_REQ_INVALID);
			goto Exit;
		}
		sr_rcv_process_delete_method(sa, p_madw);
		break;
	case IB_MAD_METHOD_GET:
	case IB_MAD_METHOD_GETTABLE:
		sr_rcv_process_get_method(sa, p_madw);
		break;
	default:
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Unsupported Method (%s) for ServiceRecord request\n",
			ib_get_sa_method_str(p_sa_mad->method));
		osm_sa_send_error(sa, p_madw, IB_MAD_STATUS_UNSUP_METHOD_ATTR);
		break;
	}

Exit:
	OSM_LOG_EXIT(sa->p_log);
}