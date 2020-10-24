void *cl_map_insert(IN cl_map_t * const p_map,
		    IN const uint64_t key, IN const void *const p_object)
{
	cl_map_obj_t *p_map_obj, *p_obj_at_key;

	CL_ASSERT(p_map);

	p_map_obj = (cl_map_obj_t *) cl_qpool_get(&p_map->pool);

	if (!p_map_obj)
		return (NULL);

	cl_qmap_set_obj(p_map_obj, p_object);

	p_obj_at_key =
	    (cl_map_obj_t *) cl_qmap_insert(&p_map->qmap, key,
					    &p_map_obj->item);

	/* Return the item to the pool if insertion failed. */
	if (p_obj_at_key != p_map_obj)
		cl_qpool_put(&p_map->pool, &p_map_obj->item.pool_item);

	return (cl_qmap_obj(p_obj_at_key));
}