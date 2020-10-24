void cl_qmap_delta(IN OUT cl_qmap_t * const p_map1,
		   IN OUT cl_qmap_t * const p_map2,
		   OUT cl_qmap_t * const p_new, OUT cl_qmap_t * const p_old)
{
	cl_map_item_t *p_item1, *p_item2;
	uint64_t key1, key2;

	CL_ASSERT(p_map1);
	CL_ASSERT(p_map2);
	CL_ASSERT(p_new);
	CL_ASSERT(p_old);
	CL_ASSERT(cl_is_qmap_empty(p_new));
	CL_ASSERT(cl_is_qmap_empty(p_old));

	p_item1 = cl_qmap_head(p_map1);
	p_item2 = cl_qmap_head(p_map2);

	while (p_item1 != cl_qmap_end(p_map1) && p_item2 != cl_qmap_end(p_map2)) {
		key1 = cl_qmap_key(p_item1);
		key2 = cl_qmap_key(p_item2);
		if (key1 < key2) {
			/* We found an old item. */
			__cl_qmap_delta_move(p_old, p_map1, &p_item1);
		} else if (key1 > key2) {
			/* We found a new item. */
			__cl_qmap_delta_move(p_new, p_map2, &p_item2);
		} else {
			/* Move both forward since they have the same key. */
			p_item1 = cl_qmap_next(p_item1);
			p_item2 = cl_qmap_next(p_item2);
		}
	}

	/* Process the remainder if the end of either source map was reached. */
	while (p_item2 != cl_qmap_end(p_map2))
		__cl_qmap_delta_move(p_new, p_map2, &p_item2);

	while (p_item1 != cl_qmap_end(p_map1))
		__cl_qmap_delta_move(p_old, p_map1, &p_item1);
}