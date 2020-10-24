cl_status_t cl_ptr_vector_set_size(IN cl_ptr_vector_t * const p_vector,
				   IN const size_t size)
{
	cl_status_t status;
	size_t new_capacity;

	CL_ASSERT(p_vector);
	CL_ASSERT(p_vector->state == CL_INITIALIZED);

	/* Check to see if the requested size is the same as the existing size. */
	if (size == p_vector->size)
		return (CL_SUCCESS);

	/* Determine if the vector has room for this element. */
	if (size >= p_vector->capacity) {
		if (!p_vector->grow_size)
			return (CL_INSUFFICIENT_MEMORY);

		/* Calculate the new capacity, taking into account the grow size. */
		new_capacity = size;
		if (size % p_vector->grow_size) {
			/* Round up to nearest grow_size boundary. */
			new_capacity += p_vector->grow_size -
			    (size % p_vector->grow_size);
		}

		status = cl_ptr_vector_set_capacity(p_vector, new_capacity);
		if (status != CL_SUCCESS)
			return (status);
	}

	p_vector->size = size;
	return (CL_SUCCESS);
}