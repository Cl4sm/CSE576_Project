void cl_vector_destroy(IN cl_vector_t * const p_vector)
{
	size_t i;
	void *p_element;

	CL_ASSERT(p_vector);
	CL_ASSERT(cl_is_state_valid(p_vector->state));

	/* Call the user's destructor for each element in the array. */
	if (p_vector->state == CL_INITIALIZED) {
		if (p_vector->pfn_dtor) {
			for (i = 0; i < p_vector->size; i++) {
				p_element = p_vector->p_ptr_array[i];
				/* Sanity check! */
				CL_ASSERT(p_element);
				p_vector->pfn_dtor(p_element,
						   (void *)p_vector->context);
			}
		}

		/* Deallocate the pages */
		while (!cl_is_qlist_empty(&p_vector->alloc_list))
			free(cl_qlist_remove_head(&p_vector->alloc_list));

		/* Destroy the page vector. */
		if (p_vector->p_ptr_array) {
			free(p_vector->p_ptr_array);
			p_vector->p_ptr_array = NULL;
		}
	}

	p_vector->state = CL_UNINITIALIZED;
}