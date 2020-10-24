cl_status_t cl_ptr_vector_set_capacity(IN cl_ptr_vector_t * const p_vector,
				       IN const size_t new_capacity)
{
	void *p_new_ptr_array;

	CL_ASSERT(p_vector);
	CL_ASSERT(p_vector->state == CL_INITIALIZED);

	/* Do we have to do anything here? */
	if (new_capacity <= p_vector->capacity) {
		/* Nope */
		return (CL_SUCCESS);
	}

	/* Allocate our pointer array. */
	p_new_ptr_array = malloc(new_capacity * sizeof(void *));
	if (!p_new_ptr_array)
		return (CL_INSUFFICIENT_MEMORY);
	else
		memset(p_new_ptr_array, 0, new_capacity * sizeof(void *));

	if (p_vector->p_ptr_array) {
		/* Copy the old pointer array into the new. */
		memcpy(p_new_ptr_array, p_vector->p_ptr_array,
		       p_vector->capacity * sizeof(void *));

		/* Free the old pointer array. */
		free((void *)p_vector->p_ptr_array);
	}

	/* Set the new array. */
	p_vector->p_ptr_array = p_new_ptr_array;

	/* Update the vector with the new capactity. */
	p_vector->capacity = new_capacity;

	return (CL_SUCCESS);
}