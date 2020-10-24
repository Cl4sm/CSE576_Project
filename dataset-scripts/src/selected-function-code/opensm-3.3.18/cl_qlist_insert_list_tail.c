void cl_qlist_insert_list_tail(IN cl_qlist_t * const p_dest_list,
			       IN cl_qlist_t * const p_src_list)
{
#if defined( _DEBUG_ )
	cl_list_item_t *p_item;
#endif

	CL_ASSERT(p_dest_list);
	CL_ASSERT(p_src_list);
	CL_ASSERT(p_dest_list->state == CL_INITIALIZED);
	CL_ASSERT(p_src_list->state == CL_INITIALIZED);

	/*
	 * Is the src list empty?
	 * We must have this check here for code below to work.
	 */
	if (cl_is_qlist_empty(p_src_list))
		return;

#if defined( _DEBUG_ )
	/* Check that all items in the source list belong there. */
	p_item = cl_qlist_head(p_src_list);
	while (p_item != cl_qlist_end(p_src_list)) {
		/* All list items in the source list must point to it. */
		CL_ASSERT(p_item->p_list == p_src_list);
		/* Point them all to the destination list. */
		p_item->p_list = p_dest_list;
		p_item = cl_qlist_next(p_item);
	}
#endif

	/* Chain the source list to the tail of the destination list. */
	cl_qlist_tail(p_dest_list)->p_next = cl_qlist_head(p_src_list);
	cl_qlist_head(p_src_list)->p_prev = cl_qlist_tail(p_dest_list);

	/*
	 * Update the tail of the destination list to the tail of
	 * the source list.
	 */
	p_dest_list->end.p_prev = cl_qlist_tail(p_src_list);
	cl_qlist_tail(p_src_list)->p_next = &p_dest_list->end;

	/*
	 * Update the count of the destination to reflect the source items having
	 * been added.
	 */
	p_dest_list->count += p_src_list->count;

	/* Update source list to reflect being empty. */
	__cl_qlist_reset(p_src_list);
}