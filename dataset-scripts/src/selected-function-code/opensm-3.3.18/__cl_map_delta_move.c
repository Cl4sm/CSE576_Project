static cl_status_t __cl_map_delta_move(OUT cl_map_t * const p_dest,
				       IN OUT cl_map_t * const p_src,
				       IN OUT cl_map_iterator_t * const p_itor)
{
	cl_map_iterator_t next;
	void *p_obj, *p_obj2;
	uint64_t key;

	/* Get a valid iterator so we can continue the loop. */
	next = cl_map_next(*p_itor);
	/* Get the pointer to the object for insertion. */
	p_obj = cl_map_obj(*p_itor);
	/* Get the key for the object. */
	key = cl_map_key(*p_itor);
	/* Move the object. */
	cl_map_remove_item(p_src, *p_itor);
	p_obj2 = cl_map_insert(p_dest, key, p_obj);
	/* Check for failure. We should never get a duplicate. */
	if (!p_obj2) {
		p_obj2 = cl_map_insert(p_src, key, p_obj);
		CL_ASSERT(p_obj2 == p_obj);
		return (CL_INSUFFICIENT_MEMORY);
	}

	/* We should never get a duplicate */
	CL_ASSERT(p_obj == p_obj2);
	/* Update the iterator so that it is valid. */
	(*p_itor) = next;

	return (CL_SUCCESS);
}