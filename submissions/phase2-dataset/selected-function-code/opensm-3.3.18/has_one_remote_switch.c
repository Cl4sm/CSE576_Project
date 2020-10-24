static boolean_t has_one_remote_switch(IN ftree_fabric_t *p_ftree,
				       IN ftree_hca_t *p_hca,
				       IN osm_node_t* p_node)
{
	boolean_t found_other_sw = FALSE;
	osm_physp_t *p_physp, *p_remote_physp;
	int i = 1;
	int ports_num;

	ports_num = osm_node_get_num_physp(p_hca->p_osm_node);
	while (!found_other_sw && (i < ports_num)) {
		p_physp = osm_node_get_physp_ptr(p_hca->p_osm_node, i);
		if (p_physp){
			p_remote_physp = p_physp->p_remote_physp;
			if (p_remote_physp && (p_remote_physp->p_node!=p_node))
				/* Found connection to sw that is not p_node */
				found_other_sw = TRUE;
		}
		i++;
	}

	return (!found_other_sw);
}