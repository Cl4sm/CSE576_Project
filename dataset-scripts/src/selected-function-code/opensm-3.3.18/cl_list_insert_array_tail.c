cl_status_t cl_list_insert_array_tail(IN cl_list_t * const p_list,
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

	/* Set the first item to add to the list. */
	p_object = (void *)p_array;

	/* Continue to add all items to the list. */
	while (items_remain--) {
		status = cl_list_insert_tail(p_list, p_object);
		if (status != CL_SUCCESS) {
			/* Remove all items that have been inserted. */
			while (items_remain++ < (item_count - 1))
				cl_list_remove_tail(p_list);
			return (status);
		}

		/* Get the next object to add to the list. */
		p_object = ((uint8_t *) p_object + item_size);
	}

	return (CL_SUCCESS);
}