void cl_fmap_merge(OUT cl_fmap_t * const p_dest_map,
		   IN OUT cl_fmap_t * const p_src_map)
{
	cl_fmap_item_t *p_item, *p_item2, *p_next;

	CL_ASSERT(p_dest_map);
	CL_ASSERT(p_src_map);

	p_item = cl_fmap_head(p_src_map);

	while (p_item != cl_fmap_end(p_src_map)) {
		p_next = cl_fmap_next(p_item);

		/* Remove the item from its current map. */
		cl_fmap_remove_item(p_src_map, p_item);
		/* Insert the item into the destination map. */
		p_item2 =
		    cl_fmap_insert(p_dest_map, cl_fmap_key(p_item), p_item);
		/* Check that the item was successfully inserted. */
		if (p_item2 != p_item) {
			/* Put the item in back in the source map. */
			p_item2 =
			    cl_fmap_insert(p_src_map, cl_fmap_key(p_item),
					   p_item);
			CL_ASSERT(p_item2 == p_item);
		}
		p_item = p_next;
	}
}