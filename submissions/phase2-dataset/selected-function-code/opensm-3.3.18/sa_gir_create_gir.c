static void sa_gir_create_gir(IN osm_sa_t * sa, IN osm_node_t * p_node,
			      IN cl_qlist_t * p_list,
			      IN ib_net64_t const match_port_guid,
			      IN ib_net16_t const match_lid,
			      IN const osm_physp_t * p_req_physp,
			      IN uint8_t const match_block_num)
{
	const osm_physp_t *p_physp;
	uint8_t port_num;
	uint8_t num_ports;
	uint16_t match_lid_ho;
	ib_net16_t base_lid_ho;
	ib_net16_t max_lid_ho;
	uint8_t lmc;
	ib_net64_t port_guid;
	uint8_t block_num, start_block_num, end_block_num, num_blocks;

	OSM_LOG_ENTER(sa->p_log);

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"Looking for GUIDRecord with LID: %u GUID:0x%016"
		PRIx64 "\n", cl_ntoh16(match_lid), cl_ntoh64(match_port_guid));

	/*
	   For switches, do not return the GUIDInfo record(s)
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
		   share a pkey. If not, continue */
		if (!osm_physp_share_pkey(sa->p_log, p_physp, p_req_physp,
					  sa->p_subn->opt.allow_both_pkeys))
			continue;

		port_guid = osm_physp_get_port_guid(p_physp);

		if (match_port_guid && (port_guid != match_port_guid))
			continue;

		/*
		   Note: the following check is a temporary workaround
		   Since 1. GUIDCap should never be 0 on ports where this applies
		   and   2. GUIDCap should not be used on ports where it doesn't apply
		   So this should really be a check for whether the port is a
		   switch external port or not!
		 */
		if (p_physp->port_info.guid_cap == 0)
			continue;

		num_blocks = p_physp->port_info.guid_cap / 8;
		if (p_physp->port_info.guid_cap % 8)
			num_blocks++;
		if (match_block_num == 255) {
			start_block_num = 0;
			end_block_num = num_blocks - 1;
		} else {
			if (match_block_num >= num_blocks)
				continue;
			end_block_num = start_block_num = match_block_num;
		}

		base_lid_ho = cl_ntoh16(osm_physp_get_base_lid(p_physp));
		match_lid_ho = cl_ntoh16(match_lid);
		if (match_lid_ho) {
			lmc = osm_physp_get_lmc(p_physp);
			max_lid_ho = (uint16_t) (base_lid_ho + (1 << lmc) - 1);

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

		for (block_num = start_block_num; block_num <= end_block_num;
		     block_num++)
			gir_rcv_new_gir(sa, p_node, p_list, port_guid,
					cl_ntoh16(base_lid_ho), p_physp,
					block_num);
	}

	OSM_LOG_EXIT(sa->p_log);
}