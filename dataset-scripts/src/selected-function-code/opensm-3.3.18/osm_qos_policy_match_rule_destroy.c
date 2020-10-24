void osm_qos_policy_match_rule_destroy(osm_qos_match_rule_t * p)
{
	unsigned i;

	if (!p)
		return;

	if (p->qos_level_name)
		free(p->qos_level_name);
	if (p->use)
		free(p->use);

	if (p->service_id_range_arr) {
		for (i = 0; i < p->service_id_range_len; i++)
			free(p->service_id_range_arr[i]);
		free(p->service_id_range_arr);
	}

	if (p->qos_class_range_arr) {
		for (i = 0; i < p->qos_class_range_len; i++)
			free(p->qos_class_range_arr[i]);
		free(p->qos_class_range_arr);
	}

	if (p->pkey_range_arr) {
		for (i = 0; i < p->pkey_range_len; i++)
			free(p->pkey_range_arr[i]);
		free(p->pkey_range_arr);
	}

	cl_list_apply_func(&p->source_list, __free_single_element, NULL);
	cl_list_remove_all(&p->source_list);
	cl_list_destroy(&p->source_list);

	cl_list_remove_all(&p->source_group_list);
	cl_list_destroy(&p->source_group_list);

	cl_list_apply_func(&p->destination_list, __free_single_element, NULL);
	cl_list_remove_all(&p->destination_list);
	cl_list_destroy(&p->destination_list);

	cl_list_remove_all(&p->destination_group_list);
	cl_list_destroy(&p->destination_group_list);

	free(p);
}