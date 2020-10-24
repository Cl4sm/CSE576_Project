cl_status_t cl_vector_set_capacity(IN cl_vector_t * const p_vector,
				   IN const size_t new_capacity)
{
	size_t new_elements;
	size_t alloc_size;
	size_t i;
	cl_list_item_t *p_buf;
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
		free(p_vector->p_ptr_array);
	}

	/* Set the new array. */
	p_vector->p_ptr_array = p_new_ptr_array;

	/*
	 * We have to add capacity to the array.  Determine how many
	 * elements to add.
	 */
	new_elements = new_capacity - p_vector->capacity;
	/* Determine the allocation size for the new array elements. */
	alloc_size = new_elements * p_vector->element_size;

	p_buf = (cl_list_item_t *) malloc(alloc_size + sizeof(cl_list_item_t));
	if (!p_buf)
		return (CL_INSUFFICIENT_MEMORY);
	else
		memset(p_buf, 0, alloc_size + sizeof(cl_list_item_t));

	cl_qlist_insert_tail(&p_vector->alloc_list, p_buf);
	/* Advance the buffer pointer past the list item. */
	p_buf++;

	for (i = p_vector->capacity; i < new_capacity; i++) {
		p_vector->p_ptr_array[i] = p_buf;
		/* Move the buffer pointer to the next element. */
		p_buf = (void *)(((uint8_t *) p_buf) + p_vector->element_size);
	}

	/* Update the vector with the new capactity. */
	p_vector->capacity = new_capacity;

	return (CL_SUCCESS);
}