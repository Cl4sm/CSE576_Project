static void hca_add_port(IN ftree_fabric_t * p_ftree,
			 IN ftree_hca_t * p_hca, IN uint8_t port_num,
			 IN uint8_t remote_port_num, IN uint16_t base_lid,
			 IN uint16_t remote_base_lid, IN ib_net64_t port_guid,
			 IN ib_net64_t remote_port_guid,
			 IN ib_net64_t remote_node_guid,
			 IN uint8_t remote_node_type,
			 IN void *p_remote_hca_or_sw, IN boolean_t is_cn,
			 IN boolean_t is_io)
{
	ftree_port_group_t *p_group;

	/* this function is supposed to be called only for adding ports
	   in hca's that lead to switches */
	CL_ASSERT(remote_node_type == IB_NODE_TYPE_SWITCH);

	p_group = hca_get_port_group_by_lid(p_hca, base_lid);

	if (!p_group) {
		p_group = port_group_create(base_lid, remote_base_lid,
					    port_guid, hca_get_guid_no(p_hca),
					    IB_NODE_TYPE_CA, p_hca,
					    remote_port_guid, remote_node_guid,
					    remote_node_type,
					    p_remote_hca_or_sw, is_cn, is_io);
		CL_ASSERT(p_group);
		p_hca->up_port_groups[p_hca->up_port_groups_num++] = p_group;
		port_group_add_port(p_group, port_num, remote_port_num);
	} else
		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR,
			"ERR AB32: Duplicated LID for CA GUID: 0x%016" PRIx64 "\n",
			cl_ntoh64(port_guid));
}				/* hca_add_port() */