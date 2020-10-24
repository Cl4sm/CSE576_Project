static cl_status_t __cl_pool_init_cb(IN void **const pp_obj,
				     IN const uint32_t count,
				     IN void *const context,
				     OUT cl_pool_item_t ** const pp_pool_item)
{
	cl_pool_t *p_pool = (cl_pool_t *) context;
	cl_pool_obj_t *p_pool_obj;
	cl_status_t status = CL_SUCCESS;

	CL_ASSERT(p_pool);
	CL_ASSERT(pp_obj);
	CL_ASSERT(count == 1);

	UNUSED_PARAM(count);

	/*
	 * Set our pointer to the list item, which is stored at the beginning of
	 * the first component.
	 */
	p_pool_obj = (cl_pool_obj_t *) * pp_obj;
	*pp_pool_item = &p_pool_obj->pool_item;

	/* Calculate the pointer to the user's first component. */
	*pp_obj = ((uint8_t *) * pp_obj) + sizeof(cl_pool_obj_t);

	/*
	 * Set the object pointer in the pool item to point to the first of the
	 * user's components.
	 */
	p_pool_obj->p_object = *pp_obj;

	/* Invoke the user's constructor callback. */
	if (p_pool->pfn_init)
		status = p_pool->pfn_init(*pp_obj, (void *)p_pool->context);

	return (status);
}