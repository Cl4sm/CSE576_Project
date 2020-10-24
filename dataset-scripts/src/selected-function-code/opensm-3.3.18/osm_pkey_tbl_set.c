ib_api_status_t osm_pkey_tbl_set(IN osm_pkey_tbl_t * p_pkey_tbl,
				 IN uint16_t block, IN ib_pkey_table_t * p_tbl,
				 IN boolean_t allow_both_pkeys)
{
	uint16_t b, i;
	ib_pkey_table_t *p_pkey_block;
	uint16_t *p_prev_pkey;
	ib_net16_t pkey, pkey_base;

	/* make sure the block is allocated */
	if (cl_ptr_vector_get_size(&p_pkey_tbl->blocks) > block)
		p_pkey_block =
		    (ib_pkey_table_t *) cl_ptr_vector_get(&p_pkey_tbl->blocks,
							  block);
	else
		p_pkey_block = NULL;

	if (!p_pkey_block) {
		p_pkey_block =
		    (ib_pkey_table_t *) malloc(sizeof(ib_pkey_table_t));
		if (!p_pkey_block)
			return IB_ERROR;
		memset(p_pkey_block, 0, sizeof(ib_pkey_table_t));
		cl_ptr_vector_set(&p_pkey_tbl->blocks, block, p_pkey_block);
	}

	/* sets the block values */
	memcpy(p_pkey_block, p_tbl, sizeof(ib_pkey_table_t));

	/*
	   NOTE: as the spec does not require uniqueness of PKeys in
	   tables there is no other way but to refresh the entire keys map.

	   Moreover, if the same key exists but with full membership it should
	   have precedence over the key with limited membership !
	 */
	cl_map_remove_all(&p_pkey_tbl->keys);

	for (b = 0; b < cl_ptr_vector_get_size(&p_pkey_tbl->blocks); b++) {

		p_pkey_block = cl_ptr_vector_get(&p_pkey_tbl->blocks, b);
		if (!p_pkey_block)
			continue;

		for (i = 0; i < IB_NUM_PKEY_ELEMENTS_IN_BLOCK; i++) {
			pkey = p_pkey_block->pkey_entry[i];
			if (ib_pkey_is_invalid(pkey))
				continue;

			if (allow_both_pkeys)
				pkey_base = pkey;
			else
				pkey_base = ib_pkey_get_base(pkey);

			/*
			   If allow_both_pkeys is FALSE,
			   ignore the PKey Full Member bit in the key but store
			   the pointer to the table element as the map value
			 */
			p_prev_pkey = cl_map_get(&p_pkey_tbl->keys, pkey_base);

			/* we only insert if no previous or it is not full member and allow_both_pkeys is FALSE */
			if ((p_prev_pkey == NULL) ||
			    (allow_both_pkeys == FALSE &&
			     cl_ntoh16(*p_prev_pkey) < cl_ntoh16(pkey)))
				cl_map_insert(&p_pkey_tbl->keys, pkey_base,
					      &(p_pkey_block->pkey_entry[i])
				    );
		}
	}
	return IB_SUCCESS;
}