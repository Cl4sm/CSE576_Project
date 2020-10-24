boolean_t cl_is_item_in_qlist(IN const cl_qlist_t * const p_list,
			      IN const cl_list_item_t * const p_list_item)
{
	const cl_list_item_t *p_temp;

	CL_ASSERT(p_list);
	CL_ASSERT(p_list_item);
	CL_ASSERT(p_list->state == CL_INITIALIZED);

	/* Traverse looking for a match */
	p_temp = cl_qlist_head(p_list);
	while (p_temp != cl_qlist_end(p_list)) {
		if (p_temp == p_list_item) {
			CL_ASSERT(p_list_item->p_list == p_list);
			return (TRUE);
		}

		p_temp = cl_qlist_next(p_temp);
	}

	return (FALSE);
}