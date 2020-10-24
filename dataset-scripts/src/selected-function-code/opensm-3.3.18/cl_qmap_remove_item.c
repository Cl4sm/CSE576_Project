void cl_qmap_remove_item(IN cl_qmap_t * const p_map,
			 IN cl_map_item_t * const p_item)
{
	cl_map_item_t *p_child, *p_del_item;

	CL_ASSERT(p_map);
	CL_ASSERT(p_map->state == CL_INITIALIZED);
	CL_ASSERT(p_item);

	if (p_item == cl_qmap_end(p_map))
		return;

	/* must be checked after comparing to cl_qmap_end, since
	   the end is not a valid item. */
	CL_ASSERT(p_item->p_map == p_map);

	if ((p_item->p_right == &p_map->nil) || (p_item->p_left == &p_map->nil)) {
		/* The item being removed has children on at most on side. */
		p_del_item = p_item;
	} else {
		/*
		 * The item being removed has children on both side.
		 * We select the item that will replace it.  After removing
		 * the substitute item and rebalancing, the tree will have the
		 * correct topology.  Exchanging the substitute for the item
		 * will finalize the removal.
		 */
		p_del_item = cl_qmap_next(p_item);
		CL_ASSERT(p_del_item != &p_map->nil);
	}

	/* Remove the item from the list. */
	__cl_primitive_remove(&p_item->pool_item.list_item);
	/* Decrement the item count. */
	p_map->count--;

	/* Get the pointer to the new root's child, if any. */
	if (p_del_item->p_left != &p_map->nil)
		p_child = p_del_item->p_left;
	else
		p_child = p_del_item->p_right;

	/*
	 * This assignment may modify the parent pointer of the nil node.
	 * This is inconsequential.
	 */
	p_child->p_up = p_del_item->p_up;
	(*__cl_map_get_parent_ptr_to_item(p_del_item)) = p_child;

	if (p_del_item->color != CL_MAP_RED)
		__cl_map_del_bal(p_map, p_child);

	/*
	 * Note that the splicing done below does not need to occur before
	 * the tree is balanced, since the actual topology changes are made by the
	 * preceding code.  The topology is preserved by the color assignment made
	 * below (reader should be reminded that p_del_item == p_item in some cases).
	 */
	if (p_del_item != p_item) {
		/*
		 * Finalize the removal of the specified item by exchanging it with
		 * the substitute which we removed above.
		 */
		p_del_item->p_up = p_item->p_up;
		p_del_item->p_left = p_item->p_left;
		p_del_item->p_right = p_item->p_right;
		(*__cl_map_get_parent_ptr_to_item(p_item)) = p_del_item;
		p_item->p_right->p_up = p_del_item;
		p_item->p_left->p_up = p_del_item;
		p_del_item->color = p_item->color;
	}

	CL_ASSERT(p_map->nil.color != CL_MAP_RED);

#ifdef _DEBUG_
	/* Clear the pointer to the map since the item has been removed. */
	p_item->p_map = NULL;
#endif
}