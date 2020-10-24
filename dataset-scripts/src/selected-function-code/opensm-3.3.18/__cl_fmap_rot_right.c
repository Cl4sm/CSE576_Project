static void __cl_fmap_rot_right(IN cl_fmap_t * const p_map,
				IN cl_fmap_item_t * const p_item)
{
	cl_fmap_item_t **pp_root;

	CL_ASSERT(p_map);
	CL_ASSERT(p_item);
	CL_ASSERT(p_item->p_left != &p_map->nil);

	/* Point R to A instead of C. */
	pp_root = __cl_fmap_get_parent_ptr_to_item(p_item);
	(*pp_root) = p_item->p_left;
	/* Set A's parent to R. */
	(*pp_root)->p_up = p_item->p_up;

	/* Set C's left to B */
	p_item->p_left = (*pp_root)->p_right;
	/*
	 * Set B's parent to C.  We trap for B being NIL since the
	 * caller may depend on NIL not changing.
	 */
	if ((*pp_root)->p_right != &p_map->nil)
		(*pp_root)->p_right->p_up = p_item;

	/* Set A's right to C. */
	(*pp_root)->p_right = p_item;
	/* Set C's parent to A. */
	p_item->p_up = *pp_root;
}