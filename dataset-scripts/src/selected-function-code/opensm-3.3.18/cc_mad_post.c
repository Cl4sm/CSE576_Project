static void cc_mad_post(osm_congestion_control_t *p_cc,
			osm_madw_t *p_madw,
			osm_node_t *p_node,
			osm_physp_t *p_physp,
			ib_net16_t attr_id,
			ib_net32_t attr_mod)
{
	osm_subn_opt_t *p_opt = &p_cc->subn->opt;
	ib_cc_mad_t *p_cc_mad;
	uint8_t port;

	OSM_LOG_ENTER(p_cc->log);

	port = osm_physp_get_port_num(p_physp);

	p_cc_mad = osm_madw_get_cc_mad_ptr(p_madw);

	p_cc_mad->header.base_ver = 1;
	p_cc_mad->header.mgmt_class = IB_MCLASS_CC;
	p_cc_mad->header.class_ver = 2;
	p_cc_mad->header.method = IB_MAD_METHOD_SET;
	p_cc_mad->header.status = 0;
	p_cc_mad->header.class_spec = 0;
	p_cc_mad->header.trans_id =
		cl_hton64((uint64_t) cl_atomic_inc(&p_cc->trans_id) &
			  (uint64_t) (0xFFFFFFFF));
	if (p_cc_mad->header.trans_id == 0)
		p_cc_mad->header.trans_id =
			cl_hton64((uint64_t) cl_atomic_inc(&p_cc->trans_id) &
				  (uint64_t) (0xFFFFFFFF));
	p_cc_mad->header.attr_id = attr_id;
	p_cc_mad->header.resv = 0;
	p_cc_mad->header.attr_mod = attr_mod;

	p_cc_mad->cc_key = p_opt->cc_key;

	memset(p_cc_mad->log_data, '\0', IB_CC_LOG_DATA_SIZE);

	p_madw->mad_addr.dest_lid = osm_node_get_base_lid(p_node, port);
	p_madw->mad_addr.addr_type.gsi.remote_qp = IB_QP1;
	p_madw->mad_addr.addr_type.gsi.remote_qkey =
		cl_hton32(IB_QP1_WELL_KNOWN_Q_KEY);
	p_madw->resp_expected = TRUE;
	p_madw->fail_msg = CL_DISP_MSGID_NONE;

	p_madw->context.cc_context.node_guid = osm_node_get_node_guid(p_node);
	p_madw->context.cc_context.port_guid = osm_physp_get_port_guid(p_physp);
	p_madw->context.cc_context.port = port;
	p_madw->context.cc_context.mad_method = IB_MAD_METHOD_SET;
	p_madw->context.cc_context.attr_mod = attr_mod;

	cl_spinlock_acquire(&p_cc->mad_queue_lock);
	cl_atomic_inc(&p_cc->outstanding_mads);
	cl_qlist_insert_tail(&p_cc->mad_queue, &p_madw->list_item);
	cl_spinlock_release(&p_cc->mad_queue_lock);

	cl_event_signal(&p_cc->cc_poller_wakeup);

	OSM_LOG_EXIT(p_cc->log);
}