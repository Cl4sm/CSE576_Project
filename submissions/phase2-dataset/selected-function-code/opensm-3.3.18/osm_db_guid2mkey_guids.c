int osm_db_guid2mkey_guids(IN osm_db_domain_t * p_g2m,
			   OUT cl_qlist_t * p_guid_list)
{
	char *p_key;
	cl_list_t keys;
	osm_db_guid_elem_t *p_guid_elem;

	cl_list_construct(&keys);
	cl_list_init(&keys, 10);

	if (osm_db_keys(p_g2m, &keys))
		return 1;

	while ((p_key = cl_list_remove_head(&keys)) != NULL) {
		p_guid_elem =
		    (osm_db_guid_elem_t *) malloc(sizeof(osm_db_guid_elem_t));
		CL_ASSERT(p_guid_elem != NULL);

		p_guid_elem->guid = unpack_guid(p_key);
		cl_qlist_insert_head(p_guid_list, &p_guid_elem->item);
	}

	cl_list_destroy(&keys);
	return 0;
}