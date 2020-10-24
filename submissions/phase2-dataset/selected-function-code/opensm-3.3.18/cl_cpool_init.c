cl_status_t cl_cpool_init(IN cl_cpool_t * const p_pool,
			  IN const size_t min_size, IN const size_t max_size,
			  IN const size_t grow_size,
			  IN size_t * const component_sizes,
			  IN const uint32_t num_components,
			  IN cl_pfn_cpool_init_t pfn_initializer OPTIONAL,
			  IN cl_pfn_cpool_dtor_t pfn_destructor OPTIONAL,
			  IN const void *const context)
{
	cl_status_t status;

	CL_ASSERT(p_pool);
	CL_ASSERT(num_components);
	CL_ASSERT(component_sizes);

	/* Add the size of the pool object to the first component. */
	component_sizes[0] += sizeof(cl_pool_obj_t);

	/* Store callback function pointers. */
	p_pool->pfn_init = pfn_initializer;	/* may be NULL */
	p_pool->pfn_dtor = pfn_destructor;	/* may be NULL */
	p_pool->context = context;

	status = cl_qcpool_init(&p_pool->qcpool, min_size, max_size, grow_size,
				component_sizes, num_components,
				__cl_cpool_init_cb,
				pfn_destructor ? __cl_cpool_dtor_cb : NULL,
				p_pool);

	/* Restore the original value of the first component. */
	component_sizes[0] -= sizeof(cl_pool_obj_t);

	return (status);
}