cl_status_t cl_map_delta(IN OUT cl_map_t * const p_map1,
			 IN OUT cl_map_t * const p_map2,
			 OUT cl_map_t * const p_new, OUT cl_map_t * const p_old)
{
	cl_map_iterator_t itor1, itor2;
	uint64_t key1, key2;
	cl_status_t status;

	CL_ASSERT(p_map1);
	CL_ASSERT(p_map2);
	CL_ASSERT(p_new);
	CL_ASSERT(p_old);
	CL_ASSERT(cl_is_map_empty(p_new));
	CL_ASSERT(cl_is_map_empty(p_old));

	itor1 = cl_map_head(p_map1);
	itor2 = cl_map_head(p_map2);

	/*
	 * Note that the check is for the end, since duplicate items will remain
	 * in their respective maps.
	 */
	while (itor1 != cl_map_end(p_map1) && itor2 != cl_map_end(p_map2)) {
		key1 = cl_map_key(itor1);
		key2 = cl_map_key(itor2);
		if (key1 < key2) {
			status = __cl_map_delta_move(p_old, p_map1, &itor1);
			/* Check for failure. */
			if (status != CL_SUCCESS) {
				/* Restore the initial state. */
				__cl_map_revert(p_map1, p_map2, p_new, p_old);
				/* Return the failure status. */
				return (status);
			}
		} else if (key1 > key2) {
			status = __cl_map_delta_move(p_new, p_map2, &itor2);
			if (status != CL_SUCCESS) {
				/* Restore the initial state. */
				__cl_map_revert(p_map1, p_map2, p_new, p_old);
				/* Return the failure status. */
				return (status);
			}
		} else {
			/* Move both forward since they have the same key. */
			itor1 = cl_map_next(itor1);
			itor2 = cl_map_next(itor2);
		}
	}

	/* Process the remainder if either source map is empty. */
	while (itor2 != cl_map_end(p_map2)) {
		status = __cl_map_delta_move(p_new, p_map2, &itor2);
		if (status != CL_SUCCESS) {
			/* Restore the initial state. */
			__cl_map_revert(p_map1, p_map2, p_new, p_old);
			/* Return the failure status. */
			return (status);
		}
	}

	while (itor1 != cl_map_end(p_map1)) {
		status = __cl_map_delta_move(p_old, p_map1, &itor1);
		if (status != CL_SUCCESS) {
			/* Restore the initial state. */
			__cl_map_revert(p_map1, p_map2, p_new, p_old);
			/* Return the failure status. */
			return (status);
		}
	}

	return (CL_SUCCESS);
}