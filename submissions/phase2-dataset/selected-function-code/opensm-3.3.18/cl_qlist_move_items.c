void cl_qlist_move_items(IN cl_qlist_t * const p_src_list,
			 IN cl_qlist_t * const p_dest_list,
			 IN cl_pfn_qlist_find_t pfn_func,
			 IN const void *const context)
{
	cl_list_item_t *p_current_item, *p_next;

	CL_ASSERT(p_src_list);
	CL_ASSERT(p_dest_list);
	CL_ASSERT(p_src_list->state == CL_INITIALIZED);
	CL_ASSERT(p_dest_list->state == CL_INITIALIZED);
	CL_ASSERT(pfn_func);

	p_current_item = cl_qlist_head(p_src_list);

	while (p_current_item != cl_qlist_end(p_src_list)) {
		/* Before we do anything, get a pointer to the next item. */
		p_next = cl_qlist_next(p_current_item);

		if (pfn_func(p_current_item, (void *)context) == CL_SUCCESS) {
			/* Move the item from one list to the other. */
			cl_qlist_remove_item(p_src_list, p_current_item);
			cl_qlist_insert_tail(p_dest_list, p_current_item);
		}
		p_current_item = p_next;
	}
}