static void fabric_set_max_cn_per_leaf(IN ftree_fabric_t * p_ftree)
{
	unsigned i;
	unsigned j;
	unsigned cns_on_this_leaf;
	ftree_sw_t *p_sw;
	ftree_port_group_t *p_group;

	for (i = 0; i < p_ftree->leaf_switches_num; i++) {
		p_sw = p_ftree->leaf_switches[i];
		cns_on_this_leaf = 0;
		for (j = 0; j < p_sw->down_port_groups_num; j++) {
			p_group = p_sw->down_port_groups[j];
			if (p_group->remote_node_type != IB_NODE_TYPE_CA)
				continue;
			cns_on_this_leaf +=
			    p_group->remote_hca_or_sw.p_hca->cn_num;
		}
		if (cns_on_this_leaf > p_ftree->max_cn_per_leaf)
			p_ftree->max_cn_per_leaf = cns_on_this_leaf;
	}
}				/* fabric_set_max_cn_per_leaf() */