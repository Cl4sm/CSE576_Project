int osm_congestion_control_setup(struct osm_opensm *p_osm)
{
	cl_qmap_t *p_tbl;
	cl_map_item_t *p_next;
	int ret = 0;

	if (!p_osm->subn.opt.congestion_control)
		return 0;

	OSM_LOG_ENTER(&p_osm->log);

	/*
	 * Do nothing unless the most recent routing attempt was successful.
	 */
	if (!p_osm->routing_engine_used)
		return 0;

	cc_setup_mad_data(&p_osm->sm);

	cl_plock_acquire(&p_osm->lock);

	p_tbl = &p_osm->subn.port_guid_tbl;
	p_next = cl_qmap_head(p_tbl);
	while (p_next != cl_qmap_end(p_tbl)) {
		osm_port_t *p_port = (osm_port_t *) p_next;
		osm_node_t *p_node = p_port->p_node;
		ib_api_status_t status;

		p_next = cl_qmap_next(p_next);

		if (p_port->cc_unavailable_flag)
			continue;

		if (osm_node_get_type(p_node) == IB_NODE_TYPE_SWITCH) {
			status = cc_send_sw_cong_setting(&p_osm->sm, p_node);
			if (status != IB_SUCCESS)
				ret = -1;
		} else if (osm_node_get_type(p_node) == IB_NODE_TYPE_CA) {
			status = cc_send_ca_cong_setting(&p_osm->sm,
							 p_node,
							 p_port->p_physp);
			if (status != IB_SUCCESS)
				ret = -1;

			status = cc_send_cct(&p_osm->sm,
					     p_node,
					     p_port->p_physp);
			if (status != IB_SUCCESS)
				ret = -1;
		}
	}

	cl_plock_release(&p_osm->lock);

	OSM_LOG_EXIT(&p_osm->log);

	return ret;
}