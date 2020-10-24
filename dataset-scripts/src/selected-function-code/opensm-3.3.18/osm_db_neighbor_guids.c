int osm_db_neighbor_guids(IN osm_db_domain_t * p_neighbor,
			  OUT cl_qlist_t * p_neighbor_list)
{
	char *p_key;
	cl_list_t keys;
	osm_db_neighbor_elem_t *p_neighbor_elem;

	cl_list_construct(&keys);
	cl_list_init(&keys, 10);

	if (osm_db_keys(p_neighbor, &keys))
		return 1;

	while ((p_key = cl_list_remove_head(&keys)) != NULL) {
		p_neighbor_elem =
		    (osm_db_neighbor_elem_t *) malloc(sizeof(osm_db_neighbor_elem_t));
		CL_ASSERT(p_neighbor_elem != NULL);

		unpack_neighbor(p_key, &p_neighbor_elem->guid,
				&p_neighbor_elem->portnum);
		cl_qlist_insert_head(p_neighbor_list, &p_neighbor_elem->item);
	}

	cl_list_destroy(&keys);
	return 0;
}