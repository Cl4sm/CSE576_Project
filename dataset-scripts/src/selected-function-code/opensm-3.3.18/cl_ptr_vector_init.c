cl_status_t cl_ptr_vector_init(IN cl_ptr_vector_t * const p_vector,
			       IN const size_t min_size,
			       IN const size_t grow_size)
{
	cl_status_t status = CL_SUCCESS;

	CL_ASSERT(p_vector);

	cl_ptr_vector_construct(p_vector);

	p_vector->grow_size = grow_size;

	/*
	 * Set the state to initialized so that the call to set_size
	 * doesn't assert.
	 */
	p_vector->state = CL_INITIALIZED;

	/* get the storage needed by the user */
	if (min_size) {
		status = cl_ptr_vector_set_size(p_vector, min_size);
		if (status != CL_SUCCESS)
			cl_ptr_vector_destroy(p_vector);
	}

	return (status);
}