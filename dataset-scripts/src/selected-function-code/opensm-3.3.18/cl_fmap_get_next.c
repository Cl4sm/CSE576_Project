cl_fmap_item_t *cl_fmap_get_next(IN const cl_fmap_t * const p_map,
				 IN const void *const p_key)
{
	cl_fmap_item_t *p_item;
	cl_fmap_item_t *p_item_found;
	int cmp;

	CL_ASSERT(p_map);
	CL_ASSERT(p_map->state == CL_INITIALIZED);

	p_item = __cl_fmap_root(p_map);
	p_item_found = (cl_fmap_item_t *) & p_map->nil;

	while (p_item != &p_map->nil) {
		cmp = p_map->pfn_compare(p_key, p_item->p_key);

		if (cmp < 0) {
			p_item_found = p_item;
			p_item = p_item->p_left;	/* too small */
		} else {
			p_item = p_item->p_right;	/* too big or match */
		}
	}

	return (p_item_found);
}