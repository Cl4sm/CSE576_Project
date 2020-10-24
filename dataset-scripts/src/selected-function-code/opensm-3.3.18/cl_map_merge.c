cl_status_t cl_map_merge(OUT cl_map_t * const p_dest_map,
			 IN OUT cl_map_t * const p_src_map)
{
	cl_status_t status = CL_SUCCESS;
	cl_map_iterator_t itor, next;
	uint64_t key;
	void *p_obj, *p_obj2;

	CL_ASSERT(p_dest_map);
	CL_ASSERT(p_src_map);

	itor = cl_map_head(p_src_map);
	while (itor != cl_map_end(p_src_map)) {
		next = cl_map_next(itor);

		p_obj = cl_map_obj(itor);
		key = cl_map_key(itor);

		cl_map_remove_item(p_src_map, itor);

		/* Insert the object into the destination map. */
		p_obj2 = cl_map_insert(p_dest_map, key, p_obj);
		/* Trap for failure. */
		if (p_obj != p_obj2) {
			if (!p_obj2)
				status = CL_INSUFFICIENT_MEMORY;
			/* Put the object back in the source map.  This must succeed. */
			p_obj2 = cl_map_insert(p_src_map, key, p_obj);
			CL_ASSERT(p_obj == p_obj2);
			/* If the failure was due to insufficient memory, return. */
			if (status != CL_SUCCESS)
				return (status);
		}
		itor = next;
	}

	return (CL_SUCCESS);
}