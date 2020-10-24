static int remove_depended_hca(IN ftree_fabric_t *p_ftree, IN ftree_sw_t *p_sw)
{
	ftree_hca_t *p_hca;
	int counter = 0;
	int port_num;
	osm_physp_t* physp;
	osm_node_t* sw_node;
	uint64_t remote_hca_guid;

	sw_node = p_sw->p_osm_sw->p_node;
	for (port_num = 0; port_num < sw_node->physp_tbl_size; port_num++) {
		physp = osm_node_get_physp_ptr(sw_node, port_num);
		if (physp && physp->p_remote_physp) {
			if (osm_node_get_type(physp->p_remote_physp->p_node) == IB_NODE_TYPE_CA) {
				remote_hca_guid =
				    osm_node_get_node_guid(physp->p_remote_physp->p_node);
				p_hca = fabric_get_hca_by_guid(p_ftree, remote_hca_guid);
				if (p_hca && has_one_remote_switch(p_ftree, p_hca, sw_node)) {
					cl_qmap_remove_item(&p_ftree->hca_tbl, &p_hca->map_item);
					hca_destroy(p_hca);
					counter++;
				}
			}
		}
	}
	return counter;
}