void osm_sminfo_rcv_process(IN void *context, IN void *data)
{
	osm_sm_t *sm = context;
	osm_madw_t *p_madw = data;
	ib_smp_t *p_smp;
	osm_smi_context_t *p_smi_context;

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_madw);

	p_smp = osm_madw_get_smp_ptr(p_madw);
	if (ib_smp_get_status(p_smp)) {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"MAD status 0x%x received\n",
			cl_ntoh16(ib_smp_get_status(p_smp)));
		goto Exit;
	}

	/* Determine if this is a request for our own SMInfo or if
	   this is a response to our request for another SM's SMInfo. */
	if (ib_smp_is_response(p_smp)) {
		const ib_sm_info_t *p_smi = ib_smp_get_payload_ptr(p_smp);

		/* Get the context - to see if this is a response to a Get or Set method */
		p_smi_context = osm_madw_get_smi_context_ptr(p_madw);

		/* Verify that response is from expected port and there is
		   no port moving issue. */
		if (p_smi_context->port_guid != p_smi->guid) {
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F19: "
				"Unexpected SM port GUID in response"
				", Expected 0x%016" PRIx64
				", Received 0x%016" PRIx64 "\n",
				cl_ntoh64(p_smi_context->port_guid),
				cl_ntoh64(p_smi->guid));
			goto Exit;
		}

		if (p_smi_context->set_method == FALSE)
			/* this is a response to a Get method */
			smi_rcv_process_get_response(sm, p_madw);
		else
			/* this is a response to a Set method */
			smi_rcv_process_set_response(sm, p_madw);
	} else if (p_smp->method == IB_MAD_METHOD_GET)
		/* This is a SubnGet request */
		smi_rcv_process_get_request(sm, p_madw);
	else
		/* This should be a SubnSet request */
		smi_rcv_process_set_request(sm, p_madw);

Exit:
	OSM_LOG_EXIT(sm->p_log);
}