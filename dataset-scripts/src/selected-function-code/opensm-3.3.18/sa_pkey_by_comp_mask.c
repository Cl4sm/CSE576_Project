static void sa_pkey_by_comp_mask(IN osm_sa_t * sa, IN const osm_port_t * p_port,
				 osm_pkey_search_ctxt_t * p_ctxt)
{
	const ib_pkey_table_record_t *p_rcvd_rec;
	ib_net64_t comp_mask;
	osm_physp_t *p_physp;
	uint8_t port_num;
	uint8_t num_ports;
	const osm_physp_t *p_req_physp;

	OSM_LOG_ENTER(sa->p_log);

	p_rcvd_rec = p_ctxt->p_rcvd_rec;
	comp_mask = p_ctxt->comp_mask;
	port_num = p_rcvd_rec->port_num;
	p_req_physp = p_ctxt->p_req_physp;

	/* if this is a switch port we can search all ports
	   otherwise we must be looking on port 0 */
	if (p_port->p_node->node_info.node_type != IB_NODE_TYPE_SWITCH) {
		/* we put it in the comp mask and port num */
		port_num = p_port->p_physp->port_num;
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Using Physical Default Port Number: 0x%X (for End Node)\n",
			port_num);
		comp_mask |= IB_PKEY_COMPMASK_PORT;
	}

	if (comp_mask & IB_PKEY_COMPMASK_PORT) {
		if (port_num < osm_node_get_num_physp(p_port->p_node)) {
			p_physp =
			    osm_node_get_physp_ptr(p_port->p_node, port_num);
			/* Check that the p_physp is valid, and that is shares a pkey
			   with the p_req_physp. */
			if (p_physp &&
			    osm_physp_share_pkey(sa->p_log, p_req_physp,
						 p_physp, sa->p_subn->opt.allow_both_pkeys))
				sa_pkey_check_physp(sa, p_physp, p_ctxt);
		} else {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4603: "
				"Given Physical Port Number: 0x%X is out of range should be < 0x%X\n",
				port_num,
				osm_node_get_num_physp(p_port->p_node));
			goto Exit;
		}
	} else {
		num_ports = osm_node_get_num_physp(p_port->p_node);
		for (port_num = 0; port_num < num_ports; port_num++) {
			p_physp =
			    osm_node_get_physp_ptr(p_port->p_node, port_num);
			if (!p_physp)
				continue;

			/* if the requester and the p_physp don't share a pkey -
			   continue */
			if (!osm_physp_share_pkey
			    (sa->p_log, p_req_physp, p_physp, sa->p_subn->opt.allow_both_pkeys))
				continue;

			sa_pkey_check_physp(sa, p_physp, p_ctxt);
		}
	}
Exit:
	OSM_LOG_EXIT(sa->p_log);
}