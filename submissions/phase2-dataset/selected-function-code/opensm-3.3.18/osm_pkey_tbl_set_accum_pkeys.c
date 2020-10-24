cl_status_t osm_pkey_tbl_set_accum_pkeys(IN osm_pkey_tbl_t * p_pkey_tbl,
					 IN uint16_t pkey,
					 IN uint16_t pkey_idx)
{
	uintptr_t ptr = pkey_idx + 1; /* 0 means not found so bias by 1 */
	uint16_t *p_prev_pkey_idx;
	cl_status_t status = CL_SUCCESS;

	if (pkey_idx >= p_pkey_tbl->last_pkey_idx)
		p_pkey_tbl->last_pkey_idx = pkey_idx + 1;

	p_prev_pkey_idx = (uint16_t *) cl_map_get(&p_pkey_tbl->accum_pkeys, pkey);

	if (p_prev_pkey_idx != NULL)
		cl_map_remove(&p_pkey_tbl->accum_pkeys, pkey);

	if (cl_map_insert(&p_pkey_tbl->accum_pkeys, pkey, (void *) ptr) == NULL)
		status = CL_INSUFFICIENT_MEMORY;

	return status;

}