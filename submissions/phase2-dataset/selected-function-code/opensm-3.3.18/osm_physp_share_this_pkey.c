boolean_t osm_physp_share_this_pkey(IN const osm_physp_t * p_physp1,
				    IN const osm_physp_t * p_physp2,
				    IN ib_net16_t pkey,
				    IN boolean_t allow_both_pkeys)
{
	ib_net16_t *pkey1, *pkey2;
	ib_net16_t full_pkey, limited_pkey;

	if (allow_both_pkeys) {
		full_pkey = pkey | IB_PKEY_TYPE_MASK;
		limited_pkey = pkey & ~IB_PKEY_TYPE_MASK;
		pkey1 = cl_map_get(&(osm_physp_get_pkey_tbl(p_physp1))->keys,
				   full_pkey);
		if (!pkey1)
			pkey1 = cl_map_get(&(osm_physp_get_pkey_tbl(p_physp1))->keys,
					   limited_pkey);
		pkey2 = cl_map_get(&(osm_physp_get_pkey_tbl(p_physp2))->keys,
				   full_pkey);
		if (!pkey2)
			pkey2 = cl_map_get(&(osm_physp_get_pkey_tbl(p_physp2))->keys,
					   limited_pkey);
	} else {
		pkey1 = cl_map_get(&(osm_physp_get_pkey_tbl(p_physp1))->keys,
				   ib_pkey_get_base(pkey));
		pkey2 = cl_map_get(&(osm_physp_get_pkey_tbl(p_physp2))->keys,
				   ib_pkey_get_base(pkey));
	}
	return (pkey1 && pkey2 && match_pkey(pkey1, pkey2));
}