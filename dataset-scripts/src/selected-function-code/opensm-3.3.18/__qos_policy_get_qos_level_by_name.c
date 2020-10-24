static osm_qos_level_t *__qos_policy_get_qos_level_by_name(
		const osm_qos_policy_t * p_qos_policy,
		const char *name)
{
	osm_qos_level_t *p_qos_level = NULL;
	cl_list_iterator_t list_iterator;

	list_iterator = cl_list_head(&p_qos_policy->qos_levels);
	while (list_iterator != cl_list_end(&p_qos_policy->qos_levels)) {
		p_qos_level = (osm_qos_level_t *) cl_list_obj(list_iterator);
		if (!p_qos_level)
			continue;

		/* names are case INsensitive */
		if (strcasecmp(name, p_qos_level->name) == 0)
			return p_qos_level;

		list_iterator = cl_list_next(list_iterator);
	}

	return NULL;
}