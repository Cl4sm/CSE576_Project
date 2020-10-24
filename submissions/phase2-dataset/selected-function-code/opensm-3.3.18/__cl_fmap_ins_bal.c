static void __cl_fmap_ins_bal(IN cl_fmap_t * const p_map,
			      IN cl_fmap_item_t * p_item)
{
	cl_fmap_item_t *p_grand_uncle;

	CL_ASSERT(p_map);
	CL_ASSERT(p_item);
	CL_ASSERT(p_item != &p_map->root);

	while (p_item->p_up->color == CL_MAP_RED) {
		if (__cl_fmap_is_left_child(p_item->p_up)) {
			p_grand_uncle = p_item->p_up->p_up->p_right;
			CL_ASSERT(p_grand_uncle);
			if (p_grand_uncle->color == CL_MAP_RED) {
				p_grand_uncle->color = CL_MAP_BLACK;
				p_item->p_up->color = CL_MAP_BLACK;
				p_item->p_up->p_up->color = CL_MAP_RED;
				p_item = p_item->p_up->p_up;
				continue;
			}

			if (!__cl_fmap_is_left_child(p_item)) {
				p_item = p_item->p_up;
				__cl_fmap_rot_left(p_map, p_item);
			}
			p_item->p_up->color = CL_MAP_BLACK;
			p_item->p_up->p_up->color = CL_MAP_RED;
			__cl_fmap_rot_right(p_map, p_item->p_up->p_up);
		} else {
			p_grand_uncle = p_item->p_up->p_up->p_left;
			CL_ASSERT(p_grand_uncle);
			if (p_grand_uncle->color == CL_MAP_RED) {
				p_grand_uncle->color = CL_MAP_BLACK;
				p_item->p_up->color = CL_MAP_BLACK;
				p_item->p_up->p_up->color = CL_MAP_RED;
				p_item = p_item->p_up->p_up;
				continue;
			}

			if (__cl_fmap_is_left_child(p_item)) {
				p_item = p_item->p_up;
				__cl_fmap_rot_right(p_map, p_item);
			}
			p_item->p_up->color = CL_MAP_BLACK;
			p_item->p_up->p_up->color = CL_MAP_RED;
			__cl_fmap_rot_left(p_map, p_item->p_up->p_up);
		}
	}
}