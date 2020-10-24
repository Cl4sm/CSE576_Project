cl_status_t cl_vector_init(IN cl_vector_t * const p_vector,
			   IN const size_t min_size, IN const size_t grow_size,
			   IN const size_t element_size,
			   IN cl_pfn_vec_init_t pfn_init OPTIONAL,
			   IN cl_pfn_vec_dtor_t pfn_dtor OPTIONAL,
			   IN const void *const context)
{
	cl_status_t status = CL_SUCCESS;

	CL_ASSERT(p_vector);
	CL_ASSERT(element_size);

	cl_vector_construct(p_vector);

	p_vector->grow_size = grow_size;
	p_vector->element_size = element_size;
	p_vector->pfn_init = pfn_init;
	p_vector->pfn_dtor = pfn_dtor;
	p_vector->context = context;

	/*
	 * Try to choose a smart copy operator
	 * someday, we could simply let the users pass one in
	 */
	switch (element_size) {
	case sizeof(uint8_t):
		p_vector->pfn_copy = cl_vector_copy8;
		break;

	case sizeof(uint16_t):
		p_vector->pfn_copy = cl_vector_copy16;
		break;

	case sizeof(uint32_t):
		p_vector->pfn_copy = cl_vector_copy32;
		break;

	case sizeof(uint64_t):
		p_vector->pfn_copy = cl_vector_copy64;
		break;

	default:
		p_vector->pfn_copy = cl_vector_copy_general;
		break;
	}

	/*
	 * Set the state to initialized so that the call to set_size
	 * doesn't assert.
	 */
	p_vector->state = CL_INITIALIZED;

	/* Initialize the allocation list */
	cl_qlist_init(&p_vector->alloc_list);

	/* get the storage needed by the user */
	if (min_size) {
		status = cl_vector_set_size(p_vector, min_size);
		if (status != CL_SUCCESS)
			cl_vector_destroy(p_vector);
	}

	return (status);
}