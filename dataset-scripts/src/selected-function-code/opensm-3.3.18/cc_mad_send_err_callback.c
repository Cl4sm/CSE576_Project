static void cc_mad_send_err_callback(void *bind_context,
				     osm_madw_t * p_madw)
{
	osm_congestion_control_t *p_cc = bind_context;
	osm_madw_context_t *p_madw_context = &p_madw->context;
	osm_opensm_t *p_osm = p_cc->osm;
	uint64_t node_guid = p_madw_context->cc_context.node_guid;
	uint64_t port_guid = p_madw_context->cc_context.port_guid;
	uint8_t port = p_madw_context->cc_context.port;
	osm_port_t *p_port;
	int log_flag = 1;

	OSM_LOG_ENTER(p_cc->log);

	cl_plock_acquire(&p_osm->lock);

	p_port = osm_get_port_by_guid(p_cc->subn, port_guid);
	if (!p_port) {
		OSM_LOG(p_cc->log, OSM_LOG_ERROR, "ERR C10B: "
			"Port guid not in table 0x%" PRIx64 "\n",
			port_guid);
		cl_plock_release(&p_osm->lock);
		goto Exit;
	}

	/* If timed out before, don't bothering logging again
	 * we assume no CC support
	 */
	if (p_madw->status == IB_TIMEOUT
	    && p_port->cc_timeout_count)
		log_flag = 0;

	if (log_flag)
		OSM_LOG(p_cc->log, OSM_LOG_ERROR, "ERR C106: MAD Error (%s): "
			"attr id = %u LID %u GUID 0x%016" PRIx64 " port %u "
			"TID 0x%" PRIx64 "\n",
			ib_get_err_str(p_madw->status),
			p_madw->p_mad->attr_id,
			cl_ntoh16(p_madw->mad_addr.dest_lid),
			node_guid,
			port,
			cl_ntoh64(p_madw->p_mad->trans_id));

	if (p_madw->status == IB_TIMEOUT) {
		p_port->cc_timeout_count++;
		if (p_port->cc_timeout_count > OSM_CC_TIMEOUT_COUNT_THRESHOLD
		    && !p_port->cc_unavailable_flag)
			p_port->cc_unavailable_flag++;
	} else
		p_cc->subn->subnet_initialization_error = TRUE;

	cl_plock_release(&p_osm->lock);

Exit:
	osm_mad_pool_put(p_cc->mad_pool, p_madw);

	decrement_outstanding_mads(p_cc);

	OSM_LOG_EXIT(p_cc->log);
}