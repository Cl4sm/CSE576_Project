cl_fmap_item_t *cl_fmap_insert(IN cl_fmap_t * const p_map,
			       IN const void *const p_key,
			       IN cl_fmap_item_t * const p_item)
{
	cl_fmap_item_t *p_insert_at, *p_comp_item;
	int cmp = 0;

	CL_ASSERT(p_map);
	CL_ASSERT(p_map->state == CL_INITIALIZED);
	CL_ASSERT(p_item);
	CL_ASSERT(p_map->root.p_up == &p_map->root);
	CL_ASSERT(p_map->root.color != CL_MAP_RED);
	CL_ASSERT(p_map->nil.color != CL_MAP_RED);

	p_item->p_left = &p_map->nil;
	p_item->p_right = &p_map->nil;
	p_item->p_key = p_key;
	p_item->color = CL_MAP_RED;

	/* Find the insertion location. */
	p_insert_at = &p_map->root;
	p_comp_item = __cl_fmap_root(p_map);

	while (p_comp_item != &p_map->nil) {
		p_insert_at = p_comp_item;

		cmp = p_map->pfn_compare(p_key, p_insert_at->p_key);

		if (!cmp)
			return (p_insert_at);

		/* Traverse the tree until the correct insertion point is found. */
		if (cmp < 0)
			p_comp_item = p_insert_at->p_left;
		else
			p_comp_item = p_insert_at->p_right;
	}

	CL_ASSERT(p_insert_at != &p_map->nil);
	CL_ASSERT(p_comp_item == &p_map->nil);
	/* Insert the item. */
	if (p_insert_at == &p_map->root) {
		p_insert_at->p_left = p_item;
		/*
		 * Primitive insert places the new item in front of
		 * the existing item.
		 */
		__cl_primitive_insert(&p_map->nil.pool_item.list_item,
				      &p_item->pool_item.list_item);
	} else if (cmp < 0) {
		p_insert_at->p_left = p_item;
		/*
		 * Primitive insert places the new item in front of
		 * the existing item.
		 */
		__cl_primitive_insert(&p_insert_at->pool_item.list_item,
				      &p_item->pool_item.list_item);
	} else {
		p_insert_at->p_right = p_item;
		/*
		 * Primitive insert places the new item in front of
		 * the existing item.
		 */
		__cl_primitive_insert(p_insert_at->pool_item.list_item.p_next,
				      &p_item->pool_item.list_item);
	}
	/* Increase the count. */
	p_map->count++;

	p_item->p_up = p_insert_at;

	/*
	 * We have added depth to this section of the tree.
	 * Rebalance as necessary as we retrace our path through the tree
	 * and update colors.
	 */
	__cl_fmap_ins_bal(p_map, p_item);

	__cl_fmap_root(p_map)->color = CL_MAP_BLACK;

	/*
	 * Note that it is not necessary to re-color the nil node black because all
	 * red color assignments are made via the p_up pointer, and nil is never
	 * set as the value of a p_up pointer.
	 */

#ifdef _DEBUG_
	/* Set the pointer to the map in the map item for consistency checking. */
	p_item->p_map = p_map;
#endif

	return (p_item);
}