void osm_pkey_tbl_destroy(IN osm_pkey_tbl_t * p_pkey_tbl)
{
	ib_pkey_table_t *p_block;
	uint16_t num_blocks, i;

	num_blocks = (uint16_t) (cl_ptr_vector_get_size(&p_pkey_tbl->blocks));
	for (i = 0; i < num_blocks; i++)
		if ((p_block = cl_ptr_vector_get(&p_pkey_tbl->blocks, i)))
			free(p_block);
	cl_ptr_vector_destroy(&p_pkey_tbl->blocks);

	num_blocks =
	    (uint16_t) (cl_ptr_vector_get_size(&p_pkey_tbl->new_blocks));
	for (i = 0; i < num_blocks; i++)
		if ((p_block = cl_ptr_vector_get(&p_pkey_tbl->new_blocks, i)))
			free(p_block);
	cl_ptr_vector_destroy(&p_pkey_tbl->new_blocks);

	cl_map_remove_all(&p_pkey_tbl->accum_pkeys);
	cl_map_destroy(&p_pkey_tbl->accum_pkeys);

	cl_map_remove_all(&p_pkey_tbl->keys);
	cl_map_destroy(&p_pkey_tbl->keys);
}