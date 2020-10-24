cl_list_item_t *cl_qlist_find_next(IN const cl_qlist_t * const p_list,
				   IN const cl_list_item_t * const p_list_item,
				   IN cl_pfn_qlist_find_t pfn_func,
				   IN const void *const context)
{
	cl_list_item_t *p_found_item;

	CL_ASSERT(p_list);
	CL_ASSERT(p_list->state == CL_INITIALIZED);
	CL_ASSERT(p_list_item);
	CL_ASSERT(p_list_item->p_list == p_list);
	CL_ASSERT(pfn_func);

	p_found_item = cl_qlist_next(p_list_item);

	/* The user provided a compare function */
	while (p_found_item != cl_qlist_end(p_list)) {
		CL_ASSERT(p_found_item->p_list == p_list);

		if (pfn_func(p_found_item, (void *)context) == CL_SUCCESS)
			break;

		p_found_item = cl_qlist_next(p_found_item);
	}

	/* No match */
	return (p_found_item);
}