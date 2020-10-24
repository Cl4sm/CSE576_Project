cl_status_t cl_pool_init(IN cl_pool_t * const p_pool, IN const size_t min_size,
			 IN const size_t max_size, IN const size_t grow_size,
			 IN const size_t object_size,
			 IN cl_pfn_pool_init_t pfn_initializer OPTIONAL,
			 IN cl_pfn_pool_dtor_t pfn_destructor OPTIONAL,
			 IN const void *const context)
{
	cl_status_t status;
	size_t total_size;

	CL_ASSERT(p_pool);

	/* Add the size of the list item to the first component. */
	total_size = object_size + sizeof(cl_pool_obj_t);

	/* Store callback function pointers. */
	p_pool->pfn_init = pfn_initializer;	/* may be NULL */
	p_pool->pfn_dtor = pfn_destructor;	/* may be NULL */
	p_pool->context = context;

	/*
	 * We need an initializer in all cases for quick composite pool, since
	 * the user pointer must be manipulated to hide the prefixed cl_pool_obj_t.
	 */
	status = cl_qcpool_init(&p_pool->qcpool, min_size, max_size, grow_size,
				&total_size, 1, __cl_pool_init_cb,
				pfn_destructor ? __cl_pool_dtor_cb : NULL,
				p_pool);

	return (status);
}