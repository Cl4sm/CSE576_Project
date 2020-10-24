static void sa_pir_by_comp_mask(IN osm_sa_t * sa, IN osm_node_t * p_node,
				osm_pir_search_ctxt_t * p_ctxt)
{
	const ib_portinfo_record_t *p_rcvd_rec;
	ib_net64_t comp_mask;
	const osm_physp_t *p_physp;
	uint8_t port_num;
	uint8_t num_ports;
	const osm_physp_t *p_req_physp;

	OSM_LOG_ENTER(sa->p_log);

	p_rcvd_rec = p_ctxt->p_rcvd_rec;
	comp_mask = p_ctxt->comp_mask;
	p_req_physp = p_ctxt->p_req_physp;

	num_ports = osm_node_get_num_physp(p_node);

	if (comp_mask & IB_PIR_COMPMASK_PORTNUM) {
		if (p_rcvd_rec->port_num < num_ports) {
			p_physp =
			    osm_node_get_physp_ptr(p_node,
						   p_rcvd_rec->port_num);
			/* Check that the p_physp is valid, and that the
			   p_physp and the p_req_physp share a pkey. */
			if (p_physp &&
			    osm_physp_share_pkey(sa->p_log, p_req_physp, p_physp,
						 sa->p_subn->opt.allow_both_pkeys))
				sa_pir_check_physp(sa, p_physp, p_ctxt);
		}
	} else {
		for (port_num = 0; port_num < num_ports; port_num++) {
			p_physp = osm_node_get_physp_ptr(p_node, port_num);
			if (!p_physp)
				continue;

			/* if the requester and the p_physp don't share a pkey -
			   continue */
			if (!osm_physp_share_pkey(sa->p_log, p_req_physp, p_physp,
						  sa->p_subn->opt.allow_both_pkeys))
				continue;

			sa_pir_check_physp(sa, p_physp, p_ctxt);
		}
	}

	OSM_LOG_EXIT(sa->p_log);
}