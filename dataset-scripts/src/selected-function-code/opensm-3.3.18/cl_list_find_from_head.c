cl_list_iterator_t cl_list_find_from_head(IN const cl_list_t * const p_list,
					  IN cl_pfn_list_find_t pfn_func,
					  IN const void *const context)
{
	cl_status_t status;
	cl_list_iterator_t itor;

	/* Note that context can have any arbitrary value. */
	CL_ASSERT(p_list);
	CL_ASSERT(cl_is_qpool_inited(&p_list->list_item_pool));
	CL_ASSERT(pfn_func);

	itor = cl_list_head(p_list);

	while (itor != cl_list_end(p_list)) {
		status = pfn_func(cl_list_obj(itor), (void *)context);
		if (status == CL_SUCCESS)
			break;

		itor = cl_list_next(itor);
	}

	/* no match */
	return (itor);
}