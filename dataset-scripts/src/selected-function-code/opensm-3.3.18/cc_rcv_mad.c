static void cc_rcv_mad(void *context, void *data)
{
	osm_congestion_control_t *p_cc = context;
	osm_opensm_t *p_osm = p_cc->osm;
	osm_madw_t *p_madw = data;
	ib_cc_mad_t *p_cc_mad;
	osm_madw_context_t *p_mad_context = &p_madw->context;
	ib_mad_t *p_mad = osm_madw_get_mad_ptr(p_madw);
	uint64_t node_guid = p_mad_context->cc_context.node_guid;
	uint64_t port_guid = p_mad_context->cc_context.port_guid;
	uint8_t port = p_mad_context->cc_context.port;
	osm_port_t *p_port;

	OSM_LOG_ENTER(p_cc->log);

	OSM_LOG(p_cc->log, OSM_LOG_VERBOSE,
		"Processing received MAD status 0x%x context 0x%"
		PRIx64 "port %u\n", p_mad->status, node_guid, port);

	p_cc_mad = osm_madw_get_cc_mad_ptr(p_madw);

	cl_plock_acquire(&p_osm->lock);

	p_port = osm_get_port_by_guid(p_cc->subn, port_guid);
	if (!p_port) {
		OSM_LOG(p_cc->log, OSM_LOG_ERROR, "ERR C109: "
			"Port guid not in table 0x%" PRIx64 "\n",
			   port_guid);
		cl_plock_release(&p_osm->lock);
		goto Exit;
	}

	p_port->cc_timeout_count = 0;

	if (p_cc_mad->header.status) {
		if (p_cc_mad->header.status & IB_MAD_STATUS_UNSUP_METHOD
		    || p_cc_mad->header.status & IB_MAD_STATUS_UNSUP_METHOD_ATTR)
			p_port->cc_unavailable_flag = 1;
		cl_plock_release(&p_osm->lock);
		goto Exit;
	}
	else
		p_port->cc_unavailable_flag = 0;

	if (p_cc_mad->header.attr_id == IB_MAD_ATTR_SW_CONG_SETTING) {
		ib_sw_cong_setting_t *p_sw_cong_setting;

		p_sw_cong_setting = ib_cc_mad_get_mgt_data_ptr(p_cc_mad);
		p_port->p_physp->cc.sw.sw_cong_setting = *p_sw_cong_setting;
	}
	else if (p_cc_mad->header.attr_id == IB_MAD_ATTR_CA_CONG_SETTING) {
		ib_ca_cong_setting_t *p_ca_cong_setting;

		p_ca_cong_setting = ib_cc_mad_get_mgt_data_ptr(p_cc_mad);
		p_port->p_physp->cc.ca.ca_cong_setting = *p_ca_cong_setting;
	}
	else if (p_cc_mad->header.attr_id == IB_MAD_ATTR_CC_TBL) {
		ib_net32_t attr_mod = p_mad_context->cc_context.attr_mod;
		uint32_t index = cl_ntoh32(attr_mod);
		ib_cc_tbl_t *p_cc_tbl;

		p_cc_tbl = ib_cc_mad_get_mgt_data_ptr(p_cc_mad);
		p_port->p_physp->cc.ca.cc_tbl[index] = *p_cc_tbl;
	}
	else
		OSM_LOG(p_cc->log, OSM_LOG_ERROR, "ERR C10A: "
			"Unexpected MAD attribute received: %u\n",
			   p_cc_mad->header.attr_id);

	cl_plock_release(&p_osm->lock);

Exit:
	decrement_outstanding_mads(p_cc);
	osm_mad_pool_put(p_cc->mad_pool, p_madw);
	OSM_LOG_EXIT(p_cc->log);
}