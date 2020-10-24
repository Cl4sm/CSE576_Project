void cl_qcpool_destroy(IN cl_qcpool_t * const p_pool)
{
	/* CL_ASSERT that a non-NULL pointer was provided. */
	CL_ASSERT(p_pool);
	/* CL_ASSERT that we are in a valid state (not uninitialized memory). */
	CL_ASSERT(cl_is_state_valid(p_pool->state));

	if (p_pool->state == CL_INITIALIZED) {
		/*
		 * Assert if the user hasn't put everything back in the pool
		 * before destroying it
		 * if they haven't, then most likely they are still using memory
		 * that will be freed, and the destructor will not be called!
		 */
#ifdef _DEBUG_
		/* but we do not want "free" version to assert on this one */
		CL_ASSERT(cl_qcpool_count(p_pool) == p_pool->num_objects);
#endif
		/* call the user's destructor for each object in the pool */
		if (p_pool->pfn_dtor) {
			while (!cl_is_qlist_empty(&p_pool->free_list)) {
				p_pool->pfn_dtor((cl_pool_item_t *)
						 cl_qlist_remove_head(&p_pool->
								      free_list),
						 (void *)p_pool->context);
			}
		} else {
			cl_qlist_remove_all(&p_pool->free_list);
		}

		/* Free all allocated memory blocks. */
		while (!cl_is_qlist_empty(&p_pool->alloc_list))
			free(cl_qlist_remove_head(&p_pool->alloc_list));

		if (p_pool->component_sizes) {
			free(p_pool->component_sizes);
			p_pool->component_sizes = NULL;
		}
	}

	p_pool->state = CL_UNINITIALIZED;
}