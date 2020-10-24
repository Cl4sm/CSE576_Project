cl_status_t cl_qcpool_init(IN cl_qcpool_t * const p_pool,
			   IN const size_t min_size, IN const size_t max_size,
			   IN const size_t grow_size,
			   IN const size_t * const component_sizes,
			   IN const uint32_t num_components,
			   IN cl_pfn_qcpool_init_t pfn_initializer OPTIONAL,
			   IN cl_pfn_qcpool_dtor_t pfn_destructor OPTIONAL,
			   IN const void *const context)
{
	cl_status_t status;
	uint32_t i;

	CL_ASSERT(p_pool);
	/* Must have a minimum of 1 component. */
	CL_ASSERT(num_components);
	/* A component size array is required. */
	CL_ASSERT(component_sizes);
	/*
	 * If no initializer is provided, the first component must be large
	 * enough to hold a pool item.
	 */
	CL_ASSERT(pfn_initializer ||
		  (component_sizes[0] >= sizeof(cl_pool_item_t)));

	cl_qcpool_construct(p_pool);

	if (num_components > 1 && !pfn_initializer)
		return (CL_INVALID_SETTING);

	if (max_size && max_size < min_size)
		return (CL_INVALID_SETTING);

	/*
	 * Allocate the array of component sizes and component pointers all
	 * in one allocation.
	 */
	p_pool->component_sizes = (size_t *) malloc((sizeof(size_t) +
						     sizeof(void *)) *
						    num_components);

	if (!p_pool->component_sizes)
		return (CL_INSUFFICIENT_MEMORY);
	else
		memset(p_pool->component_sizes, 0,
		       (sizeof(size_t) + sizeof(void *)) * num_components);

	/* Calculate the pointer to the array of pointers, used for callbacks. */
	p_pool->p_components =
	    (void **)(p_pool->component_sizes + num_components);

	/* Copy the user's sizes into our array for future use. */
	memcpy(p_pool->component_sizes, component_sizes,
	       sizeof(component_sizes[0]) * num_components);

	/* Store the number of components per object. */
	p_pool->num_components = num_components;

	/* Round up and store the size of the components. */
	for (i = 0; i < num_components; i++) {
		/*
		 * We roundup each component size so that all components
		 * are aligned on a natural boundary.
		 */
		p_pool->component_sizes[i] =
		    ROUNDUP(p_pool->component_sizes[i], sizeof(uintptr_t));
	}

	p_pool->max_objects = max_size ? max_size : ~(size_t) 0;
	p_pool->grow_size = grow_size;

	/* Store callback function pointers. */
	p_pool->pfn_init = pfn_initializer;	/* may be NULL */
	p_pool->pfn_dtor = pfn_destructor;	/* may be NULL */
	p_pool->context = context;

	cl_qlist_init(&p_pool->alloc_list);

	cl_qlist_init(&p_pool->free_list);

	/*
	 * We are now initialized.  We change the initialized flag before
	 * growing since the grow function asserts that we are initialized.
	 */
	p_pool->state = CL_INITIALIZED;

	/* Allocate the minimum number of objects as requested. */
	if (!min_size)
		return (CL_SUCCESS);

	status = cl_qcpool_grow(p_pool, min_size);
	/* Trap for error and cleanup if necessary. */
	if (status != CL_SUCCESS)
		cl_qcpool_destroy(p_pool);

	return (status);
}