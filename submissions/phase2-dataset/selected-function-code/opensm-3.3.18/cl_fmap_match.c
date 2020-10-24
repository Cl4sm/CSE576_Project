cl_fmap_item_t *cl_fmap_match(IN const cl_fmap_t * const p_map,
			      IN const void *const p_key,
			      IN cl_pfn_fmap_cmp_t pfn_compare)
{
	cl_fmap_item_t *p_item;
	int cmp;

	CL_ASSERT(p_map);
	CL_ASSERT(p_map->state == CL_INITIALIZED);

	p_item = __cl_fmap_root(p_map);

	while (p_item != &p_map->nil) {
		cmp = pfn_compare ? pfn_compare(p_key, p_item->p_key) :
			p_map->pfn_compare(p_key, p_item->p_key);

		if (!cmp)
			break;	/* just right */

		if (cmp < 0)
			p_item = p_item->p_left;	/* too small */
		else
			p_item = p_item->p_right;	/* too big */
	}

	return (p_item);
}