cl_status_t cl_qpool_init(IN cl_qpool_t * const p_pool,
			  IN const size_t min_size, IN const size_t max_size,
			  IN const size_t grow_size,
			  IN const size_t object_size,
			  IN cl_pfn_qpool_init_t pfn_initializer OPTIONAL,
			  IN cl_pfn_qpool_dtor_t pfn_destructor OPTIONAL,
			  IN const void *const context)
{
	cl_status_t status;

	CL_ASSERT(p_pool);

	p_pool->pfn_init = pfn_initializer;	/* may be NULL */
	p_pool->pfn_dtor = pfn_destructor;	/* may be NULL */
	p_pool->context = context;

	status = cl_qcpool_init(&p_pool->qcpool, min_size, max_size, grow_size,
				&object_size, 1,
				pfn_initializer ? __cl_qpool_init_cb : NULL,
				pfn_destructor ? __cl_qpool_dtor_cb : NULL,
				p_pool);

	return (status);
}