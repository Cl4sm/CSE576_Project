static void sa_slvl_by_comp_mask(IN osm_sa_t * sa, IN const osm_port_t * p_port,
				 osm_slvl_search_ctxt_t * p_ctxt)
{
	const ib_slvl_table_record_t *p_rcvd_rec;
	ib_net64_t comp_mask;
	const osm_physp_t *p_out_physp, *p_in_physp;
	uint8_t in_port_num, out_port_num;
	uint8_t num_ports;
	uint8_t in_port_start, in_port_end;
	uint8_t out_port_start, out_port_end;
	const osm_physp_t *p_req_physp;

	OSM_LOG_ENTER(sa->p_log);

	p_rcvd_rec = p_ctxt->p_rcvd_rec;
	comp_mask = p_ctxt->comp_mask;
	num_ports = osm_node_get_num_physp(p_port->p_node);
	in_port_start = 0;
	in_port_end = num_ports - 1;
	out_port_start = 0;
	out_port_end = num_ports - 1;
	p_req_physp = p_ctxt->p_req_physp;

	if (p_port->p_node->node_info.node_type != IB_NODE_TYPE_SWITCH) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Using Physical Default Port Number: 0x%X (for End Node)\n",
			p_port->p_physp->port_num);
		p_out_physp = p_port->p_physp;
		/* check that the p_out_physp and the p_req_physp share a pkey */
		if (osm_physp_share_pkey(sa->p_log, p_req_physp, p_out_physp,
					 sa->p_subn->opt.allow_both_pkeys))
			sa_slvl_create(sa, p_out_physp, p_ctxt, 0);
	} else {
		if (comp_mask & IB_SLVL_COMPMASK_OUT_PORT)
			out_port_start = out_port_end =
			    p_rcvd_rec->out_port_num;
		if (comp_mask & IB_SLVL_COMPMASK_IN_PORT)
			in_port_start = in_port_end = p_rcvd_rec->in_port_num;

		for (out_port_num = out_port_start;
		     out_port_num <= out_port_end; out_port_num++) {
			p_out_physp =
			    osm_node_get_physp_ptr(p_port->p_node,
						   out_port_num);
			if (!p_out_physp)
				continue;

			for (in_port_num = in_port_start;
			     in_port_num <= in_port_end; in_port_num++) {
#if 0
				if (out_port_num && out_port_num == in_port_num)
					continue;
#endif

				p_in_physp =
				    osm_node_get_physp_ptr(p_port->p_node,
							   in_port_num);
				if (!p_in_physp)
					continue;

				/* if the requester and the p_out_physp don't share a pkey -
				   continue */
				if (!osm_physp_share_pkey(sa->p_log, p_req_physp, p_out_physp,
							  sa->p_subn->opt.allow_both_pkeys))
					continue;

				sa_slvl_create(sa, p_out_physp, p_ctxt,
					       in_port_num);
			}
		}
	}
	OSM_LOG_EXIT(sa->p_log);
}