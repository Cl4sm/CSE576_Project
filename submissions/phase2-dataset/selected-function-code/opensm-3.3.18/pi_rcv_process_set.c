static void pi_rcv_process_set(IN osm_sm_t * sm, IN osm_node_t * p_node,
			       IN uint8_t port_num, IN osm_madw_t * p_madw)
{
	osm_physp_t *p_physp;
	ib_net64_t port_guid;
	ib_smp_t *p_smp;
	ib_port_info_t *p_pi;
	osm_pi_context_t *p_context;
	osm_log_level_t level;

	OSM_LOG_ENTER(sm->p_log);

	p_context = osm_madw_get_pi_context_ptr(p_madw);

	CL_ASSERT(p_node);

	p_physp = osm_node_get_physp_ptr(p_node, port_num);
	CL_ASSERT(p_physp);

	port_guid = osm_physp_get_port_guid(p_physp);

	p_smp = osm_madw_get_smp_ptr(p_madw);
	p_pi = ib_smp_get_payload_ptr(p_smp);

	/* check for error */
	if (cl_ntoh16(p_smp->status) & 0x7fff) {
		/* If port already ACTIVE, don't treat status 7 as error */
		if (p_context->active_transition &&
		    (cl_ntoh16(p_smp->status) & 0x7fff) == 0x1c) {
			level = OSM_LOG_INFO;
			OSM_LOG(sm->p_log, OSM_LOG_INFO,
				"Received error status 0x%x for SetResp() during ACTIVE transition\n",
				cl_ntoh16(p_smp->status) & 0x7fff);
			/* Should there be a subsequent Get to validate that port is ACTIVE ? */
		} else {
			level = OSM_LOG_ERROR;
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0F10: "
				"Received error status for SetResp()\n");
		}
		osm_dump_port_info_v2(sm->p_log, osm_node_get_node_guid(p_node),
				      port_guid, port_num, p_pi, FILE_ID, level);
	}

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
		"Received logical SetResp() for GUID 0x%" PRIx64
		", port num %u"
		"\n\t\t\t\tfor parent node GUID 0x%" PRIx64
		" TID 0x%" PRIx64 "\n",
		cl_ntoh64(port_guid), port_num,
		cl_ntoh64(osm_node_get_node_guid(p_node)),
		cl_ntoh64(p_smp->trans_id));

	osm_physp_set_port_info(p_physp, p_pi, sm);

	OSM_LOG_EXIT(sm->p_log);
}