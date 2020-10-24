void cl_fmap_init(IN cl_fmap_t * const p_map, IN cl_pfn_fmap_cmp_t pfn_compare)
{
	CL_ASSERT(p_map);
	CL_ASSERT(pfn_compare);

	memset(p_map, 0, sizeof(cl_fmap_t));

	/* special setup for the root node */
	p_map->root.p_up = &p_map->root;
	p_map->root.p_left = &p_map->nil;
	p_map->root.p_right = &p_map->nil;
	p_map->root.color = CL_MAP_BLACK;

	/* Setup the node used as terminator for all leaves. */
	p_map->nil.p_up = &p_map->nil;
	p_map->nil.p_left = &p_map->nil;
	p_map->nil.p_right = &p_map->nil;
	p_map->nil.color = CL_MAP_BLACK;

	/* Store the compare function pointer. */
	p_map->pfn_compare = pfn_compare;

	p_map->state = CL_INITIALIZED;

	cl_fmap_remove_all(p_map);
}