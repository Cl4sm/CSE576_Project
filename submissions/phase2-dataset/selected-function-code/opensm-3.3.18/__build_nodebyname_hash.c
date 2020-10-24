static void
__build_nodebyname_hash(osm_qos_policy_t * p_qos_policy)
{
	osm_node_t * p_node;
	cl_qmap_t  * p_node_guid_tbl = &p_qos_policy->p_subn->node_guid_tbl;

	p_qos_policy->p_node_hash = st_init_strtable();
	CL_ASSERT(p_qos_policy->p_node_hash);

	if (!p_node_guid_tbl || !cl_qmap_count(p_node_guid_tbl))
		return;

	for (p_node = (osm_node_t *) cl_qmap_head(p_node_guid_tbl);
	     p_node != (osm_node_t *) cl_qmap_end(p_node_guid_tbl);
	     p_node = (osm_node_t *) cl_qmap_next(&p_node->map_item)) {
		if (!st_lookup(p_qos_policy->p_node_hash,
			      (st_data_t)p_node->print_desc, NULL))
			st_insert(p_qos_policy->p_node_hash,
				  (st_data_t)p_node->print_desc,
				  (st_data_t)p_node);
	}
}