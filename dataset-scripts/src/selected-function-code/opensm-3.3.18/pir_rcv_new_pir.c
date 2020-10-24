static ib_api_status_t pir_rcv_new_pir(IN osm_sa_t * sa,
				       IN const osm_physp_t * p_physp,
				       IN osm_pir_search_ctxt_t * p_ctxt,
				       IN ib_net16_t const lid)
{
	osm_sa_item_t *p_rec_item;
	ib_port_info_t *p_pi;
	osm_physp_t *p_physp0;
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(sa->p_log);

	p_rec_item = malloc(SA_PIR_RESP_SIZE);
	if (p_rec_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2102: "
			"rec_item alloc failed\n");
		status = IB_INSUFFICIENT_RESOURCES;
		goto Exit;
	}

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"New PortInfoRecord: port 0x%016" PRIx64
		", lid %u, port %u\n",
		cl_ntoh64(osm_physp_get_port_guid(p_physp)),
		cl_ntoh16(lid), osm_physp_get_port_num(p_physp));

	memset(p_rec_item, 0, SA_PIR_RESP_SIZE);

	p_rec_item->resp.port_rec.lid = lid;
	p_rec_item->resp.port_rec.port_info = p_physp->port_info;
	if (p_ctxt->comp_mask & IB_PIR_COMPMASK_OPTIONS)
		p_rec_item->resp.port_rec.options = p_ctxt->p_rcvd_rec->options;
	if ((p_ctxt->comp_mask & IB_PIR_COMPMASK_OPTIONS) == 0 ||
	    (p_ctxt->p_rcvd_rec->options & 0x80) == 0) {
		/* Does requested port have an extended link speed active ? */
		if (osm_node_get_type(p_physp->p_node) ==
		    IB_NODE_TYPE_SWITCH) {
			p_physp0 = osm_node_get_physp_ptr(p_physp->p_node, 0);
			p_pi = &p_physp0->port_info;
		} else
			p_pi = (ib_port_info_t *) &p_physp->port_info;
		if ((p_pi->capability_mask & IB_PORT_CAP_HAS_EXT_SPEEDS) > 0) {
			if (ib_port_info_get_link_speed_ext_active(&p_physp->port_info)) {
				/* Add QDR bits to original link speed components */
				p_pi = &p_rec_item->resp.port_rec.port_info;
				ib_port_info_set_link_speed_enabled(p_pi,
								    ib_port_info_get_link_speed_enabled(p_pi) | IB_LINK_SPEED_ACTIVE_10);
				p_pi->state_info1 =
				    (uint8_t) ((p_pi->state_info1 & IB_PORT_STATE_MASK) |
					       (ib_port_info_get_link_speed_sup(p_pi) | IB_LINK_SPEED_ACTIVE_10) << IB_PORT_LINK_SPEED_SHIFT);
				p_pi->link_speed =
				    (uint8_t) ((p_pi->link_speed & IB_PORT_LINK_SPEED_ENABLED_MASK) |
					       (ib_port_info_get_link_speed_active(p_pi) | IB_LINK_SPEED_ACTIVE_10) << IB_PORT_LINK_SPEED_SHIFT);
			}
		}
	}
	p_rec_item->resp.port_rec.port_num = osm_physp_get_port_num(p_physp);

	cl_qlist_insert_tail(p_ctxt->p_list, &p_rec_item->list_item);

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return status;
}