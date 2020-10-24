size_t cl_ptr_vector_find_from_end(IN const cl_ptr_vector_t * const p_vector,
				   IN cl_pfn_ptr_vec_find_t pfn_callback,
				   IN const void *const context)
{
	size_t i;

	CL_ASSERT(p_vector);
	CL_ASSERT(p_vector->state == CL_INITIALIZED);
	CL_ASSERT(pfn_callback);

	i = p_vector->size;

	while (i) {
		/* Invoke the callback for the current element. */
		i--;
		if (pfn_callback(i, (void *)p_vector->p_ptr_array[i],
				 (void *)context) == CL_SUCCESS) {
			return (i);
		}
	}

	return (p_vector->size);
}