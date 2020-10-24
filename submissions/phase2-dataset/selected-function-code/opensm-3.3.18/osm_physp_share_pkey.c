boolean_t osm_physp_share_pkey(IN osm_log_t * p_log,
			       IN const osm_physp_t * p_physp_1,
			       IN const osm_physp_t * p_physp_2,
			       IN boolean_t allow_both_pkeys)
{
	const osm_pkey_tbl_t *pkey_tbl1, *pkey_tbl2;

	if (p_physp_1 == p_physp_2)
		return TRUE;

	pkey_tbl1 = osm_physp_get_pkey_tbl(p_physp_1);
	pkey_tbl2 = osm_physp_get_pkey_tbl(p_physp_2);

	/*
	   The spec: 10.9.2 does not require each phys port to have PKey Table.
	   So actually if it does not, we need to use the default port instead.

	   HACK: meanwhile we will ignore the check
	 */
	if (cl_is_map_empty(&pkey_tbl1->keys)
	    || cl_is_map_empty(&pkey_tbl2->keys))
		return TRUE;

	return
	    !ib_pkey_is_invalid(osm_physp_find_common_pkey
				(p_physp_1, p_physp_2, allow_both_pkeys));
}