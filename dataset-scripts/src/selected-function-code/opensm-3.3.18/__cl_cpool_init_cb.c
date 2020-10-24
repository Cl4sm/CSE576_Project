static cl_status_t __cl_cpool_init_cb(IN void **const p_comp_array,
				      IN const uint32_t num_components,
				      IN void *const context,
				      OUT cl_pool_item_t ** const pp_pool_item)
{
	cl_cpool_t *p_pool = (cl_cpool_t *) context;
	cl_pool_obj_t *p_pool_obj;
	cl_status_t status = CL_SUCCESS;

	CL_ASSERT(p_pool);

	/*
	 * Set our pointer to the list item, which is stored at the beginning of
	 * the first component.
	 */
	p_pool_obj = (cl_pool_obj_t *) p_comp_array[0];
	/* Set the pool item pointer for the caller. */
	*pp_pool_item = &p_pool_obj->pool_item;

	/* Calculate the pointer to the user's first component. */
	p_comp_array[0] = ((uint8_t *) p_comp_array[0]) + sizeof(cl_pool_obj_t);

	/*
	 * Set the object pointer in the pool object to point to the first of the
	 * user's components.
	 */
	p_pool_obj->p_object = p_comp_array[0];

	/* Invoke the user's constructor callback. */
	if (p_pool->pfn_init) {
		status = p_pool->pfn_init(p_comp_array, num_components,
					  (void *)p_pool->context);
	}

	return (status);
}