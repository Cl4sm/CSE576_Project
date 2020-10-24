cl_status_t cl_ptr_vector_set(IN cl_ptr_vector_t * const p_vector,
			      IN const size_t index,
			      IN const void *const element)
{
	cl_status_t status;

	CL_ASSERT(p_vector);
	CL_ASSERT(p_vector->state == CL_INITIALIZED);

	/* Determine if the vector has room for this element. */
	if (index >= p_vector->size) {
		/* Resize to accomodate the given index. */
		status = cl_ptr_vector_set_size(p_vector, index + 1);

		/* Check for failure on or before the given index. */
		if ((status != CL_SUCCESS) && (p_vector->size < index))
			return (status);
	}

	/* At this point, the array is guaranteed to be big enough */
	p_vector->p_ptr_array[index] = element;

	return (CL_SUCCESS);
}