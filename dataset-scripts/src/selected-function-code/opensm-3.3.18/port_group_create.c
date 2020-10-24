static ftree_port_group_t *port_group_create(IN uint16_t base_lid,
					     IN uint16_t remote_base_lid,
					     IN ib_net64_t port_guid,
					     IN ib_net64_t node_guid,
					     IN uint8_t node_type,
					     IN void *p_hca_or_sw,
					     IN ib_net64_t remote_port_guid,
					     IN ib_net64_t remote_node_guid,
					     IN uint8_t remote_node_type,
					     IN void *p_remote_hca_or_sw,
					     IN boolean_t is_cn,
					     IN boolean_t is_io)
{
	ftree_port_group_t *p_group =
	    (ftree_port_group_t *) malloc(sizeof(ftree_port_group_t));
	if (p_group == NULL)
		return NULL;
	memset(p_group, 0, sizeof(ftree_port_group_t));

	p_group->base_lid = base_lid;
	p_group->remote_base_lid = remote_base_lid;
	memcpy(&p_group->port_guid, &port_guid, sizeof(ib_net64_t));
	memcpy(&p_group->node_guid, &node_guid, sizeof(ib_net64_t));
	memcpy(&p_group->remote_port_guid, &remote_port_guid,
	       sizeof(ib_net64_t));
	memcpy(&p_group->remote_node_guid, &remote_node_guid,
	       sizeof(ib_net64_t));

	p_group->node_type = node_type;
	switch (node_type) {
	case IB_NODE_TYPE_CA:
		p_group->hca_or_sw.p_hca = (ftree_hca_t *) p_hca_or_sw;
		break;
	case IB_NODE_TYPE_SWITCH:
		p_group->hca_or_sw.p_sw = (ftree_sw_t *) p_hca_or_sw;
		break;
	default:
		/* we shouldn't get here - port is created only in hca or switch */
		CL_ASSERT(0);
	}

	p_group->remote_node_type = remote_node_type;
	switch (remote_node_type) {
	case IB_NODE_TYPE_CA:
		p_group->remote_hca_or_sw.p_hca =
		    (ftree_hca_t *) p_remote_hca_or_sw;
		break;
	case IB_NODE_TYPE_SWITCH:
		p_group->remote_hca_or_sw.p_sw =
		    (ftree_sw_t *) p_remote_hca_or_sw;
		break;
	default:
		/* we shouldn't get here - port is created only in hca or switch */
		CL_ASSERT(0);
	}

	cl_ptr_vector_init(&p_group->ports, 0,	/* min size */
			   8);	/* grow size */
	p_group->is_cn = is_cn;
	p_group->is_io = is_io;
	return p_group;
}				/* port_group_create() */