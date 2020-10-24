cl_pool_item_t *cl_qcpool_get_tail(IN cl_qcpool_t * const p_pool)
{
	cl_list_item_t *p_list_item;

	CL_ASSERT(p_pool);
	CL_ASSERT(p_pool->state == CL_INITIALIZED);

	if (cl_is_qlist_empty(&p_pool->free_list)) {
		/*
		 * No object is available.
		 * Return NULL if the user does not want automatic growth.
		 */
		if (!p_pool->grow_size)
			return (NULL);

		/* We ran out of elements.  Get more */
		cl_qcpool_grow(p_pool, p_pool->grow_size);
		/*
		 * We may not have gotten everything we wanted but we might have
		 * gotten something.
		 */
		if (cl_is_qlist_empty(&p_pool->free_list))
			return (NULL);
	}

	p_list_item = cl_qlist_remove_tail(&p_pool->free_list);
	/* OK, at this point we have an object */
	CL_ASSERT(p_list_item != cl_qlist_end(&p_pool->free_list));
	return ((cl_pool_item_t *) p_list_item);
}