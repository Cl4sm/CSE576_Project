osm_qos_policy_t * osm_qos_policy_create(osm_subn_t * p_subn)
{
	osm_qos_policy_t * p_qos_policy = (osm_qos_policy_t *)calloc(1, sizeof(osm_qos_policy_t));
	if (!p_qos_policy)
		return NULL;

	cl_list_construct(&p_qos_policy->port_groups);
	cl_list_init(&p_qos_policy->port_groups, 10);

	cl_list_construct(&p_qos_policy->vlarb_tables);
	cl_list_init(&p_qos_policy->vlarb_tables, 10);

	cl_list_construct(&p_qos_policy->sl2vl_tables);
	cl_list_init(&p_qos_policy->sl2vl_tables, 10);

	cl_list_construct(&p_qos_policy->qos_levels);
	cl_list_init(&p_qos_policy->qos_levels, 10);

	cl_list_construct(&p_qos_policy->qos_match_rules);
	cl_list_init(&p_qos_policy->qos_match_rules, 10);

	p_qos_policy->p_subn = p_subn;
	__build_nodebyname_hash(p_qos_policy);

	return p_qos_policy;
}