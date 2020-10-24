static osm_qos_port_group_t *__qos_policy_get_port_group_by_name(
		const osm_qos_policy_t * p_qos_policy,
		const char *const name)
{
	osm_qos_port_group_t *p_port_group = NULL;
	cl_list_iterator_t list_iterator;

	list_iterator = cl_list_head(&p_qos_policy->port_groups);
	while (list_iterator != cl_list_end(&p_qos_policy->port_groups)) {
		p_port_group =
		    (osm_qos_port_group_t *) cl_list_obj(list_iterator);
		if (!p_port_group)
			continue;

		/* names are case INsensitive */
		if (strcasecmp(name, p_port_group->name) == 0)
			return p_port_group;

		list_iterator = cl_list_next(list_iterator);
	}

	return NULL;
}