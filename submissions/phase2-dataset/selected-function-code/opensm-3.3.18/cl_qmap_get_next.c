cl_map_item_t *cl_qmap_get_next(IN const cl_qmap_t * const p_map,
				IN const uint64_t key)
{
	cl_map_item_t *p_item;
	cl_map_item_t *p_item_found;

	CL_ASSERT(p_map);
	CL_ASSERT(p_map->state == CL_INITIALIZED);

	p_item = __cl_map_root(p_map);
	p_item_found = (cl_map_item_t *) & p_map->nil;

	while (p_item != &p_map->nil) {
		if (key < p_item->key) {
			p_item_found = p_item;
			p_item = p_item->p_left;
		} else {
			p_item = p_item->p_right;
		}
	}

	return (p_item_found);
}