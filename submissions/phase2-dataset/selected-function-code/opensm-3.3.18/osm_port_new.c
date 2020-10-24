osm_port_t *osm_port_new(IN const ib_node_info_t * p_ni,
			 IN osm_node_t * p_parent_node)
{
	osm_port_t *p_port;
	ib_net64_t port_guid;
	osm_physp_t *p_physp;
	uint8_t port_num;

	p_port = malloc(sizeof(*p_port));
	if (!p_port)
		return NULL;

	memset(p_port, 0, sizeof(*p_port));
	cl_qlist_init(&p_port->mcm_list);
	p_port->p_node = (struct osm_node *)p_parent_node;
	port_guid = p_ni->port_guid;
	p_port->guid = port_guid;
	port_num = p_ni->node_type == IB_NODE_TYPE_SWITCH ?
	    0 : ib_node_info_get_local_port_num(p_ni);

	/*
	   Get the pointers to the physical node objects "owned" by this
	   logical port GUID.
	   For switches, port '0' is owned; for HCA's and routers,
	   only the singular part that has this GUID is owned.
	 */
	p_physp = osm_node_get_physp_ptr(p_parent_node, port_num);
	CL_ASSERT(port_guid == osm_physp_get_port_guid(p_physp));
	p_port->p_physp = p_physp;

	return p_port;
}