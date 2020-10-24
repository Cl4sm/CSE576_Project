static void nr_rcv_create_nr(IN osm_sa_t * sa, IN osm_node_t * p_node,
			     IN cl_qlist_t * p_list,
			     IN ib_net64_t const match_port_guid,
			     IN ib_net16_t const match_lid,
			     IN unsigned int const match_port_num,
			     IN const osm_physp_t * p_req_physp,
			     IN const ib_net64_t comp_mask)
{
	const osm_physp_t *p_physp;
	uint8_t port_num;
	uint8_t num_ports;
	uint16_t match_lid_ho;
	ib_net16_t base_lid;
	ib_net16_t base_lid_ho;
	ib_net16_t max_lid_ho;
	uint8_t lmc;
	ib_net64_t port_guid;

	OSM_LOG_ENTER(sa->p_log);

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"Looking for NodeRecord with LID: %u GUID: 0x%016"
		PRIx64 "\n", cl_ntoh16(match_lid), cl_ntoh64(match_port_guid));

	/*
	   For switches, do not return the NodeInfo record
	   for each port on the switch, just for port 0.
	 */
	if (osm_node_get_type(p_node) == IB_NODE_TYPE_SWITCH)
		num_ports = 1;
	else
		num_ports = osm_node_get_num_physp(p_node);

	for (port_num = 0; port_num < num_ports; port_num++) {
		p_physp = osm_node_get_physp_ptr(p_node, port_num);
		if (!p_physp)
			continue;

		/* Check to see if the found p_physp and the requester physp
		   share a pkey. If not - continue */
		if (!osm_physp_share_pkey(sa->p_log, p_physp, p_req_physp,
					  sa->p_subn->opt.allow_both_pkeys))
			continue;

		port_guid = osm_physp_get_port_guid(p_physp);

		if ((comp_mask & IB_NR_COMPMASK_PORTGUID)
		    && (port_guid != match_port_guid))
			continue;

		base_lid = osm_physp_get_base_lid(p_physp);

		if (comp_mask & IB_NR_COMPMASK_LID) {
			base_lid_ho = cl_ntoh16(base_lid);
			lmc = osm_physp_get_lmc(p_physp);
			max_lid_ho = (uint16_t) (base_lid_ho + (1 << lmc) - 1);
			match_lid_ho = cl_ntoh16(match_lid);

			/*
			   We validate that the lid belongs to this node.
			 */
			OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
				"Comparing LID: %u <= %u <= %u\n",
				base_lid_ho, match_lid_ho, max_lid_ho);

			if (match_lid_ho < base_lid_ho
			    || match_lid_ho > max_lid_ho)
				continue;
		}

		if ((comp_mask & IB_NR_COMPMASK_PORTNUM) &&
		    (port_num != match_port_num))
			continue;

		nr_rcv_new_nr(sa, p_node, p_list, port_guid, base_lid, port_num);
	}

	OSM_LOG_EXIT(sa->p_log);
}