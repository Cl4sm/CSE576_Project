cl_status_t cl_qcpool_grow(IN cl_qcpool_t * const p_pool, IN size_t obj_count)
{
	cl_status_t status = CL_SUCCESS;
	uint8_t *p_objects;
	cl_pool_item_t *p_pool_item;
	uint32_t i;
	size_t obj_size;

	CL_ASSERT(p_pool);
	CL_ASSERT(p_pool->state == CL_INITIALIZED);
	CL_ASSERT(obj_count);

	/* Validate that growth is possible. */
	if (p_pool->num_objects == p_pool->max_objects)
		return (CL_INSUFFICIENT_MEMORY);

	/* Cap the growth to the desired maximum. */
	if (obj_count > (p_pool->max_objects - p_pool->num_objects))
		obj_count = p_pool->max_objects - p_pool->num_objects;

	/* Calculate the size of an object. */
	obj_size = 0;
	for (i = 0; i < p_pool->num_components; i++)
		obj_size += p_pool->component_sizes[i];

	/* Allocate the buffer for the new objects. */
	p_objects = (uint8_t *)
	    malloc(sizeof(cl_list_item_t) + (obj_size * obj_count));

	/* Make sure the allocation succeeded. */
	if (!p_objects)
		return (CL_INSUFFICIENT_MEMORY);
	else
		memset(p_objects, 0,
		       sizeof(cl_list_item_t) + (obj_size * obj_count));

	/* Insert the allocation in our list. */
	cl_qlist_insert_tail(&p_pool->alloc_list, (cl_list_item_t *) p_objects);
	p_objects += sizeof(cl_list_item_t);

	/* initialize the new elements and add them to the free list */
	while (obj_count--) {
		/* Setup the array of components for the current object. */
		p_pool->p_components[0] = p_objects;
		for (i = 1; i < p_pool->num_components; i++) {
			/* Calculate the pointer to the next component. */
			p_pool->p_components[i] =
			    (uint8_t *) p_pool->p_components[i - 1] +
			    p_pool->component_sizes[i - 1];
		}

		/*
		 * call the user's initializer
		 * this can fail!
		 */
		if (p_pool->pfn_init) {
			p_pool_item = NULL;
			status = p_pool->pfn_init(p_pool->p_components,
						  p_pool->num_components,
						  (void *)p_pool->context,
						  &p_pool_item);
			if (status != CL_SUCCESS) {
				/*
				 * User initialization failed
				 * we may have only grown the pool by some partial amount
				 * Invoke the destructor for the object that failed
				 * initialization.
				 */
				if (p_pool->pfn_dtor)
					p_pool->pfn_dtor(p_pool_item,
							 (void *)p_pool->
							 context);

				/* Return the user's status. */
				return (status);
			}
			CL_ASSERT(p_pool_item);
		} else {
			/*
			 * If no initializer is provided, assume that the pool item
			 * is stored at the beginning of the first component.
			 */
			p_pool_item =
			    (cl_pool_item_t *) p_pool->p_components[0];
		}

#ifdef _DEBUG_
		/*
		 * Set the pool item's pool pointer to this pool so that we can
		 * check that items get returned to the correct pool.
		 */
		p_pool_item->p_pool = p_pool;
#endif

		/* Insert the new item in the free list, traping for failure. */
		cl_qlist_insert_head(&p_pool->free_list,
				     &p_pool_item->list_item);

		p_pool->num_objects++;

		/* move the pointer to the next item */
		p_objects += obj_size;
	}

	return (status);
}