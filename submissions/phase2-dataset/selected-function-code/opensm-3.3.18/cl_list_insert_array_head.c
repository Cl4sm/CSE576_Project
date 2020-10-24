cl_status_t cl_list_insert_array_head(IN cl_list_t * const p_list,
				      IN const void *const p_array,
				      IN uint32_t item_count,
				      IN const uint32_t item_size)
{
	cl_status_t status;
	void *p_object;
	uint32_t items_remain = item_count;

	CL_ASSERT(p_list);
	CL_ASSERT(cl_is_qpool_inited(&p_list->list_item_pool));
	CL_ASSERT(p_array);
	CL_ASSERT(item_size);
	CL_ASSERT(item_count);

	/*
	 * To add items from the array to the list in the same order as
	 * the elements appear in the array, we add them starting with
	 * the last one first.  Locate the last item.
	 */
	p_object = ((uint8_t *) p_array + (item_size * (item_count - 1)));

	/* Continue to add all items to the list. */
	while (items_remain--) {
		status = cl_list_insert_head(p_list, p_object);
		if (status != CL_SUCCESS) {
			/* Remove all items that have been inserted. */
			while (items_remain++ < (item_count - 1))
				cl_list_remove_head(p_list);
			return (status);
		}

		/* Get the next object to add to the list. */
		p_object = ((uint8_t *) p_object - item_size);
	}

	return (CL_SUCCESS);
}