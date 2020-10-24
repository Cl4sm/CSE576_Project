void cl_qlist_insert_array_tail(IN cl_qlist_t * const p_list,
				IN cl_list_item_t * const p_array,
				IN uint32_t item_count,
				IN const uint32_t item_size)
{
	cl_list_item_t *p_item;

	CL_ASSERT(p_list);
	CL_ASSERT(p_list->state == CL_INITIALIZED);
	CL_ASSERT(p_array);
	CL_ASSERT(item_size >= sizeof(cl_list_item_t));
	CL_ASSERT(item_count);

	/* Set the first item to add to the list. */
	p_item = p_array;

	/* Continue to add all items to the list. */
	while (item_count--) {
		cl_qlist_insert_tail(p_list, p_item);

		/* Get the next object to add to the list. */
		p_item = (cl_list_item_t *) ((uint8_t *) p_item + item_size);
	}
}