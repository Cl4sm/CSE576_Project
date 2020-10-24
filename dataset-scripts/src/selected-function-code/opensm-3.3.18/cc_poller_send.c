static void cc_poller_send(osm_congestion_control_t *p_cc,
			   osm_madw_t *p_madw)
{
	osm_subn_opt_t *p_opt = &p_cc->subn->opt;
	ib_api_status_t status;
	cl_status_t sts;
	osm_madw_context_t mad_context = p_madw->context;

	status = osm_vendor_send(p_cc->bind_handle, p_madw, TRUE);
	if (status == IB_SUCCESS) {
		cl_atomic_inc(&p_cc->outstanding_mads_on_wire);
		while (p_cc->outstanding_mads_on_wire >
		       (int32_t)p_opt->cc_max_outstanding_mads) {
wait:
			sts = cl_event_wait_on(&p_cc->sig_mads_on_wire_continue,
					       EVENT_NO_TIMEOUT, TRUE);
			if (sts != CL_SUCCESS)
				goto wait;
		}
	} else
		OSM_LOG(p_cc->log, OSM_LOG_ERROR, "ERR C104: "
			"send failed to node 0x%" PRIx64 "port %u\n",
			mad_context.cc_context.node_guid,
			mad_context.cc_context.port);
}