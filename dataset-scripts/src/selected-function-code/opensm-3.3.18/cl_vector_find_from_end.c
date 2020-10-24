size_t cl_vector_find_from_end(IN const cl_vector_t * const p_vector,
			       IN cl_pfn_vec_find_t pfn_callback,
			       IN const void *const context)
{
	size_t i;
	void *p_element;

	CL_ASSERT(p_vector);
	CL_ASSERT(p_vector->state == CL_INITIALIZED);
	CL_ASSERT(pfn_callback);

	i = p_vector->size;

	while (i) {
		/* Get a pointer to the element in the array. */
		p_element = cl_vector_get_ptr(p_vector, --i);
		CL_ASSERT(p_element);

		/* Invoke the callback for the current element. */
		if (pfn_callback(i, p_element, (void *)context) == CL_SUCCESS)
			return (i);
	}

	return (p_vector->size);
}