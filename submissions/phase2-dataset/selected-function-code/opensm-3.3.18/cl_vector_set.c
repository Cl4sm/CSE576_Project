cl_status_t cl_vector_set(IN cl_vector_t * const p_vector,
			  IN const size_t index, IN void *const p_element)
{
	cl_status_t status;
	void *p_dest;

	CL_ASSERT(p_vector);
	CL_ASSERT(p_vector->state == CL_INITIALIZED);
	CL_ASSERT(p_element);

	/* Determine if the vector has room for this element. */
	if (index >= p_vector->size) {
		/* Resize to accomodate the given index. */
		status = cl_vector_set_size(p_vector, index + 1);

		/* Check for failure on or before the given index. */
		if ((status != CL_SUCCESS) && (p_vector->size < index))
			return (status);
	}

	/* At this point, the array is guaranteed to be big enough */
	p_dest = cl_vector_get_ptr(p_vector, index);
	/* Sanity check! */
	CL_ASSERT(p_dest);

	/* Copy the data into the array */
	p_vector->pfn_copy(p_dest, p_element, p_vector->element_size);

	return (CL_SUCCESS);
}