static int fabric_construct_sw_ports(IN ftree_fabric_t * p_ftree,
				     IN ftree_sw_t * p_sw)
{
	ftree_hca_t *p_remote_hca;
	ftree_sw_t *p_remote_sw;
	osm_node_t *p_node = p_sw->p_osm_sw->p_node;
	osm_node_t *p_remote_node;
	uint16_t remote_base_lid;
	uint8_t remote_node_type;
	ib_net64_t remote_node_guid;
	osm_physp_t *p_remote_osm_port;
	ftree_direction_t direction;
	void *p_remote_hca_or_sw;
	uint8_t i;
	uint8_t remote_port_num;
	int res = 0;

	CL_ASSERT(osm_node_get_type(p_node) == IB_NODE_TYPE_SWITCH);

	for (i = 1; i < osm_node_get_num_physp(p_node); i++) {
		osm_physp_t *p_osm_port = osm_node_get_physp_ptr(p_node, i);
		if (!p_osm_port || !osm_link_is_healthy(p_osm_port))
			continue;

		p_remote_osm_port = osm_physp_get_remote(p_osm_port);
		if (!p_remote_osm_port)
			continue;

		p_remote_node =
		    osm_node_get_remote_node(p_node, i, &remote_port_num);
		if (!p_remote_node)
			continue;

		/* ignore any loopback connection on switch */
		if (p_node == p_remote_node) {
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
				"Ignoring loopback on switch GUID 0x%016" PRIx64
				", LID %u, rank %u\n",
				sw_get_guid_ho(p_sw),
				p_sw->base_lid, p_sw->rank);
			continue;
		}

		remote_node_type = osm_node_get_type(p_remote_node);
		remote_node_guid = osm_node_get_node_guid(p_remote_node);

		switch (remote_node_type) {
		case IB_NODE_TYPE_ROUTER:
			/* leaving this port - proceeding to the next one */
			continue;

		case IB_NODE_TYPE_CA:
			/* switch connected to hca */

			p_remote_hca =
			    fabric_get_hca_by_guid(p_ftree, remote_node_guid);
			CL_ASSERT(p_remote_hca);

			p_remote_hca_or_sw = (void *)p_remote_hca;
			direction = FTREE_DIRECTION_DOWN;

			remote_base_lid =
			    cl_ntoh16(osm_physp_get_base_lid
				      (p_remote_osm_port));
			break;

		case IB_NODE_TYPE_SWITCH:
			/* switch connected to another switch */

			p_remote_sw =
			    fabric_get_sw_by_guid(p_ftree, remote_node_guid);
			CL_ASSERT(p_remote_sw);

			p_remote_hca_or_sw = (void *)p_remote_sw;

			if (p_sw->rank > p_remote_sw->rank) {
				direction = FTREE_DIRECTION_UP;
			} else if (p_sw->rank == p_remote_sw->rank) {
				direction = FTREE_DIRECTION_SAME;
			} else
				direction = FTREE_DIRECTION_DOWN;

			/* switch LID is only in port 0 port_info structure */
			remote_base_lid =
			    cl_ntoh16(osm_node_get_base_lid(p_remote_node, 0));

			break;

		default:
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR,
				"ERR AB13: Node GUID 0x%016" PRIx64
				" - Unknown node type: %s\n",
				cl_ntoh64(remote_node_guid),
				ib_get_node_type_str(remote_node_type));
			res = -1;
			goto Exit;
		}
		sw_add_port(p_sw,	/* local ftree_sw object */
			    i,	/* local port number */
			    remote_port_num,	/* remote port number */
			    p_sw->base_lid,	/* local lid */
			    remote_base_lid,	/* remote lid */
			    osm_physp_get_port_guid(p_osm_port),	/* local port guid */
			    osm_physp_get_port_guid(p_remote_osm_port),	/* remote port guid */
			    remote_node_guid,	/* remote node guid */
			    remote_node_type,	/* remote node type */
			    p_remote_hca_or_sw,	/* remote ftree_hca/sw object */
			    direction);	/* port direction (up or down) */

		/* Track the max lid (in host order) that exists in the fabric */
		if (remote_base_lid > p_ftree->lft_max_lid)
			p_ftree->lft_max_lid = remote_base_lid;
	}

Exit:
	return res;
}				/* fabric_construct_sw_ports() */