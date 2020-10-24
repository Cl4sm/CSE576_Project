static void __cl_map_del_bal(IN cl_qmap_t * const p_map,
			     IN cl_map_item_t * p_item)
{
	cl_map_item_t *p_uncle;

	while ((p_item->color != CL_MAP_RED) && (p_item->p_up != &p_map->root)) {
		if (__cl_map_is_left_child(p_item)) {
			p_uncle = p_item->p_up->p_right;

			if (p_uncle->color == CL_MAP_RED) {
				p_uncle->color = CL_MAP_BLACK;
				p_item->p_up->color = CL_MAP_RED;
				__cl_map_rot_left(p_map, p_item->p_up);
				p_uncle = p_item->p_up->p_right;
			}

			if (p_uncle->p_right->color != CL_MAP_RED) {
				if (p_uncle->p_left->color != CL_MAP_RED) {
					p_uncle->color = CL_MAP_RED;
					p_item = p_item->p_up;
					continue;
				}

				p_uncle->p_left->color = CL_MAP_BLACK;
				p_uncle->color = CL_MAP_RED;
				__cl_map_rot_right(p_map, p_uncle);
				p_uncle = p_item->p_up->p_right;
			}
			p_uncle->color = p_item->p_up->color;
			p_item->p_up->color = CL_MAP_BLACK;
			p_uncle->p_right->color = CL_MAP_BLACK;
			__cl_map_rot_left(p_map, p_item->p_up);
			break;
		} else {
			p_uncle = p_item->p_up->p_left;

			if (p_uncle->color == CL_MAP_RED) {
				p_uncle->color = CL_MAP_BLACK;
				p_item->p_up->color = CL_MAP_RED;
				__cl_map_rot_right(p_map, p_item->p_up);
				p_uncle = p_item->p_up->p_left;
			}

			if (p_uncle->p_left->color != CL_MAP_RED) {
				if (p_uncle->p_right->color != CL_MAP_RED) {
					p_uncle->color = CL_MAP_RED;
					p_item = p_item->p_up;
					continue;
				}

				p_uncle->p_right->color = CL_MAP_BLACK;
				p_uncle->color = CL_MAP_RED;
				__cl_map_rot_left(p_map, p_uncle);
				p_uncle = p_item->p_up->p_left;
			}
			p_uncle->color = p_item->p_up->color;
			p_item->p_up->color = CL_MAP_BLACK;
			p_uncle->p_left->color = CL_MAP_BLACK;
			__cl_map_rot_right(p_map, p_item->p_up);
			break;
		}
	}
	p_item->color = CL_MAP_BLACK;
}