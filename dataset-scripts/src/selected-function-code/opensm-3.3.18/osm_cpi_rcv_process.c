void osm_cpi_rcv_process(IN void *context, IN void *data)
{
	osm_sa_t *sa = context;
	osm_madw_t *p_madw = data;
	const ib_sa_mad_t *p_sa_mad;

	OSM_LOG_ENTER(sa->p_log);

	CL_ASSERT(p_madw);

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);

	/* we only support GET */
	if (p_sa_mad->method != IB_MAD_METHOD_GET) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1403: "
			"Unsupported Method (%s) for ClassPortInfo request\n",
			ib_get_sa_method_str(p_sa_mad->method));
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_REQ_INVALID);
		goto Exit;
	}

	CL_ASSERT(p_sa_mad->attr_id == IB_MAD_ATTR_CLASS_PORT_INFO);

	/* CLASS PORT INFO does not really look at the SMDB - no lock required. */

	cpi_rcv_respond(sa, p_madw);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}