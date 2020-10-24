cl_map_item_t *cl_qmap_get(IN const cl_qmap_t * const p_map,
			   IN const uint64_t key)
{
	cl_map_item_t *p_item;

	CL_ASSERT(p_map);
	CL_ASSERT(p_map->state == CL_INITIALIZED);

	p_item = __cl_map_root(p_map);

	while (p_item != &p_map->nil) {
		if (key == p_item->key)
			break;	/* just right */

		if (key < p_item->key)
			p_item = p_item->p_left;	/* too small */
		else
			p_item = p_item->p_right;	/* too big */
	}

	return (p_item);
}