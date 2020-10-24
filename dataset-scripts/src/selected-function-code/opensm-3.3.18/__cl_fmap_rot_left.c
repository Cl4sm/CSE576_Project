static void __cl_fmap_rot_left(IN cl_fmap_t * const p_map,
			       IN cl_fmap_item_t * const p_item)
{
	cl_fmap_item_t **pp_root;

	CL_ASSERT(p_map);
	CL_ASSERT(p_item);
	CL_ASSERT(p_item->p_right != &p_map->nil);

	pp_root = __cl_fmap_get_parent_ptr_to_item(p_item);

	/* Point R to C instead of A. */
	*pp_root = p_item->p_right;
	/* Set C's parent to R. */
	(*pp_root)->p_up = p_item->p_up;

	/* Set A's right to B */
	p_item->p_right = (*pp_root)->p_left;
	/*
	 * Set B's parent to A.  We trap for B being NIL since the
	 * caller may depend on NIL not changing.
	 */
	if ((*pp_root)->p_left != &p_map->nil)
		(*pp_root)->p_left->p_up = p_item;

	/* Set C's left to A. */
	(*pp_root)->p_left = p_item;
	/* Set A's parent to C. */
	p_item->p_up = *pp_root;
}