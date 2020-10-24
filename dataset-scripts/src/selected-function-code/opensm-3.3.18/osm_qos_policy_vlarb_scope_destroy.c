void osm_qos_policy_vlarb_scope_destroy(osm_qos_vlarb_scope_t * p)
{
	if (!p)
		return;

	cl_list_apply_func(&p->group_list, __free_single_element, NULL);
	cl_list_apply_func(&p->across_list, __free_single_element, NULL);
	cl_list_apply_func(&p->vlarb_high_list, __free_single_element, NULL);
	cl_list_apply_func(&p->vlarb_low_list, __free_single_element, NULL);

	cl_list_remove_all(&p->group_list);
	cl_list_remove_all(&p->across_list);
	cl_list_remove_all(&p->vlarb_high_list);
	cl_list_remove_all(&p->vlarb_low_list);

	cl_list_destroy(&p->group_list);
	cl_list_destroy(&p->across_list);
	cl_list_destroy(&p->vlarb_high_list);
	cl_list_destroy(&p->vlarb_low_list);

	free(p);
}