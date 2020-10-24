osm_node_t *osm_node_new(IN const osm_madw_t * p_madw)
{
	osm_node_t *p_node;
	ib_smp_t *p_smp;
	ib_node_info_t *p_ni;
	uint8_t i;
	uint32_t size;

	p_smp = osm_madw_get_smp_ptr(p_madw);
	p_ni = ib_smp_get_payload_ptr(p_smp);

	/*
	   The node object already contains one physical port object.
	   Therefore, subtract 1 from the number of physical ports
	   used by the switch.  This is not done for CA's since they
	   need to occupy 1 more physp than they physically have since
	   we still reserve room for a "port 0".
	 */
	size = p_ni->num_ports;

	p_node = malloc(sizeof(*p_node) + sizeof(osm_physp_t) * size);
	if (!p_node)
		return NULL;

	memset(p_node, 0, sizeof(*p_node) + sizeof(osm_physp_t) * size);
	p_node->node_info = *p_ni;
	p_node->physp_tbl_size = size + 1;

	p_node->physp_discovered = malloc(sizeof(uint8_t) * p_node->physp_tbl_size);
	if (!p_node->physp_discovered) {
		free(p_node);
		return NULL;
	}
	memset(p_node->physp_discovered, 0, sizeof(uint8_t) * p_node->physp_tbl_size);
	/*
	   Construct Physical Port objects owned by this Node.
	   Then, initialize the Physical Port through with we
	   discovered this port.
	   For switches, all ports have the same GUID.
	   For CAs and routers, each port has a different GUID, so we only
	   know the GUID for the port that responded to our
	   Get(NodeInfo).
	 */
	for (i = 0; i < p_node->physp_tbl_size; i++)
		osm_physp_construct(&p_node->physp_table[i]);

	if (p_ni->node_type == IB_NODE_TYPE_SWITCH)
		for (i = 0; i <= p_ni->num_ports; i++)
			osm_node_init_physp(p_node, i, p_madw);
	else
		osm_node_init_physp(p_node,
				    ib_node_info_get_local_port_num(p_ni),
				    p_madw);
	p_node->print_desc = strdup(OSM_NODE_DESC_UNKNOWN);

	return p_node;
}