static boolean_t
__qos_policy_is_port_in_group_list(const osm_qos_policy_t * p_qos_policy,
				   const osm_physp_t * p_physp,
				   cl_list_t * p_port_group_list)
{
	osm_qos_port_group_t *p_port_group;
	cl_list_iterator_t list_iterator;

	list_iterator = cl_list_head(p_port_group_list);
	while (list_iterator != cl_list_end(p_port_group_list)) {
		p_port_group =
		    (osm_qos_port_group_t *) cl_list_obj(list_iterator);
		if (p_port_group) {
			if (__qos_policy_is_port_in_group
			    (p_qos_policy->p_subn, p_physp, p_port_group))
				return TRUE;
		}
		list_iterator = cl_list_next(list_iterator);
	}
	return FALSE;
}