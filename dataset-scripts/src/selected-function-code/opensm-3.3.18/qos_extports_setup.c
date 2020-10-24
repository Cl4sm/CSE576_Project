static int qos_extports_setup(osm_sm_t * sm, osm_node_t *node,
			      const struct qos_config *qcfg,
			      cl_qlist_t *port_mad_list)

{
	osm_physp_t *p0, *p;
	unsigned force_update;
	unsigned num_ports = osm_node_get_num_physp(node);
	struct osm_routing_engine *re = sm->p_subn->p_osm->routing_engine_used;
	int ret = 0;
	unsigned in, out;
	uint8_t op_vl, common_op_vl = 0, max_num = 0;
	uint8_t op_vl_arr[15];

	/*
	 * Do nothing unless the most recent routing attempt was successful.
	 */
	if (!re)
		return ret;

	for (out = 1; out < num_ports; out++) {
		p = osm_node_get_physp_ptr(node, out);
		if (ib_port_info_get_port_state(&p->port_info) == IB_LINK_DOWN)
			continue;
		force_update = p->need_update || sm->p_subn->need_update;
		p->vl_high_limit = qcfg->vl_high_limit;
		if (vlarb_update(sm, p, p->port_num, force_update, qcfg,
				 port_mad_list))
			ret = -1;
	}

	p0 = osm_node_get_physp_ptr(node, 0);
	if (!(p0->port_info.capability_mask & IB_PORT_CAP_HAS_SL_MAP))
		return ret;

	if (ib_switch_info_get_opt_sl2vlmapping(&node->sw->switch_info) &&
	    sm->p_subn->opt.use_optimized_slvl && !re->update_sl2vl) {

		/* we should find the op_vl that is used by majority of ports */
		memset(&op_vl_arr[0], 0, sizeof(op_vl_arr));
		p0 = osm_node_get_physp_ptr(node, 1);

		for (out = 1; out < num_ports; out++) {
			p = osm_node_get_physp_ptr(node, out);
			if (ib_port_info_get_port_state(&p->port_info) ==
			    IB_LINK_DOWN)
				continue;
			op_vl = ib_port_info_get_op_vls(&p->port_info);
			op_vl_arr[op_vl]++;
			if (op_vl_arr[op_vl] > max_num){
				max_num = op_vl_arr[op_vl];
				common_op_vl = op_vl;
				/* remember the port with most common op_vl */
				p0 = p;
			}

		}
		force_update = node->sw->need_update || sm->p_subn->need_update;
		if (sl2vl_update_table(sm, p0, p0->port_num, 0x30000, force_update,
					&qcfg->sl2vl, port_mad_list))
			ret = -1;
		/*
		 * Overwrite default ALL configuration if port's
		 * op_vl is different.
		 */
		for (out = 1; out < num_ports; out++) {
			p = osm_node_get_physp_ptr(node, out);
			if (ib_port_info_get_port_state(&p->port_info) ==
			    IB_LINK_DOWN)
				continue;

			force_update = p->need_update || force_update;
			if (ib_port_info_get_op_vls(&p->port_info) !=
			    common_op_vl &&
			    sl2vl_update_table(sm, p, p->port_num, 0x20000 | out,
					       force_update, &qcfg->sl2vl,
					       port_mad_list))
				ret = -1;
		}
		return ret;
	}

	/* non optimized sl2vl configuration */
	out = ib_switch_info_is_enhanced_port0(&node->sw->switch_info) ? 0 : 1;
	for (; out < num_ports; out++) {
		p = osm_node_get_physp_ptr(node, out);
		if (ib_port_info_get_port_state(&p->port_info) == IB_LINK_DOWN)
			continue;
		force_update = p->need_update || sm->p_subn->need_update;
		/* go over all in ports */
		for (in = 0; in < num_ports; in++) {
			const ib_slvl_table_t *port_sl2vl = &qcfg->sl2vl;
			ib_slvl_table_t routing_sl2vl;

			if (re->update_sl2vl) {
				routing_sl2vl = *port_sl2vl;
				re->update_sl2vl(re->context,
						 p, in, out, &routing_sl2vl);
				port_sl2vl = &routing_sl2vl;
			}
			if (sl2vl_update_table(sm, p, in, in << 8 | out,
					       force_update, port_sl2vl,
					       port_mad_list))
				ret = -1;
		}
	}

	return ret;
}