cl_status_t cl_vector_set_size(IN cl_vector_t * const p_vector,
			       IN const size_t size)
{
	cl_status_t status;
	size_t new_capacity;
	size_t index;
	void *p_element;

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

		status = cl_vector_set_capacity(p_vector, new_capacity);
		if (status != CL_SUCCESS)
			return (status);
	}

	/* Are we growing the array and need to invoke an initializer callback? */
	if (size > p_vector->size && p_vector->pfn_init) {
		for (index = p_vector->size; index < size; index++) {
			/* Get a pointer to this element */
			p_element = cl_vector_get_ptr(p_vector, index);

			/* Call the user's initializer and trap failures. */
			status =
			    p_vector->pfn_init(p_element,
					       (void *)p_vector->context);
			if (status != CL_SUCCESS) {
				/* Call the destructor for this object */
				if (p_vector->pfn_dtor)
					p_vector->pfn_dtor(p_element,
							   (void *)p_vector->
							   context);

				/* Return the failure status to the caller. */
				return (status);
			}

			/* The array just grew by one element */
			p_vector->size++;
		}
	} else if (p_vector->pfn_dtor) {
		/* The array is shrinking and there is a destructor to invoke. */
		for (index = size; index < p_vector->size; index++) {
			/* compute the address of the new elements */
			p_element = cl_vector_get_ptr(p_vector, index);
			/* call the user's destructor */
			p_vector->pfn_dtor(p_element,
					   (void *)p_vector->context);
		}
	}

	p_vector->size = size;
	return (CL_SUCCESS);
}