static osm_qos_level_t * __qos_policy_get_qos_level_by_params(
	IN const osm_qos_policy_t * p_qos_policy,
	IN const osm_physp_t * p_src_physp,
	IN const osm_physp_t * p_dest_physp,
	IN uint64_t service_id,
	IN uint16_t qos_class,
	IN uint16_t pkey,
	IN ib_net64_t comp_mask)
{
	osm_qos_match_rule_t *p_qos_match_rule = NULL;

	if (!p_qos_policy)
		return NULL;

	p_qos_match_rule = __qos_policy_get_match_rule_by_params(
		p_qos_policy, service_id, qos_class, pkey,
		p_src_physp, p_dest_physp, comp_mask);

	return p_qos_match_rule ? p_qos_match_rule->p_qos_level :
		p_qos_policy->p_default_qos_level;
}				/* __qos_policy_get_qos_level_by_params() */