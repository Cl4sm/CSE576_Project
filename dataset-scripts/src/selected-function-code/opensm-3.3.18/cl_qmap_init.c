void cl_qmap_init(IN cl_qmap_t * const p_map)
{
	CL_ASSERT(p_map);

	memset(p_map, 0, sizeof(cl_qmap_t));

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

	p_map->state = CL_INITIALIZED;

	cl_qmap_remove_all(p_map);
}