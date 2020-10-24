ib_net16_t osm_physp_find_common_pkey(IN const osm_physp_t * p_physp1,
				      IN const osm_physp_t * p_physp2,
				      IN boolean_t allow_both_pkeys)
{
	ib_net16_t *pkey1, *pkey2;
	uint64_t pkey1_base, pkey2_base;
	const osm_pkey_tbl_t *pkey_tbl1, *pkey_tbl2;
	cl_map_iterator_t map_iter1, map_iter2;

	pkey_tbl1 = osm_physp_get_pkey_tbl(p_physp1);
	pkey_tbl2 = osm_physp_get_pkey_tbl(p_physp2);

	map_iter1 = cl_map_head(&pkey_tbl1->keys);
	map_iter2 = cl_map_head(&pkey_tbl2->keys);

	/* we rely on the fact the map are sorted by pkey */
	while ((map_iter1 != cl_map_end(&pkey_tbl1->keys)) &&
	       (map_iter2 != cl_map_end(&pkey_tbl2->keys))) {
		pkey1 = (ib_net16_t *) cl_map_obj(map_iter1);
		pkey2 = (ib_net16_t *) cl_map_obj(map_iter2);

		if (match_pkey(pkey1, pkey2))
			return *pkey1;

		/* advance the lower value if they are not equal */
		pkey1_base = cl_map_key(map_iter1);
		pkey2_base = cl_map_key(map_iter2);
		if (pkey2_base == pkey1_base) {
			map_iter1 = cl_map_next(map_iter1);
			map_iter2 = cl_map_next(map_iter2);
		} else if (pkey2_base < pkey1_base)
			map_iter2 = cl_map_next(map_iter2);
		else
			map_iter1 = cl_map_next(map_iter1);
	}

	if (!allow_both_pkeys)
		return 0;

	/*
	   When using allow_both_pkeys, the keys in pkey tables are the
	   pkey value including membership bit.
	   Therefore, in order to complete the search, we also need to
	   compare port\s 1 full pkeys with port 2 limited pkeys, and
	   port 2 full pkeys with port 1 full pkeys.
	*/

	map_iter1 = cl_map_head(&pkey_tbl1->keys);
	map_iter2 = cl_map_head(&pkey_tbl2->keys);

	/* comparing pkey_tbl1 full with pkey_tbl2 limited */
	while ((map_iter1 != cl_map_end(&pkey_tbl1->keys)) &&
	       (map_iter2 != cl_map_end(&pkey_tbl2->keys))) {
		pkey1 = (ib_net16_t *) cl_map_obj(map_iter1);
		pkey2 = (ib_net16_t *) cl_map_obj(map_iter2);

		if (!ib_pkey_is_full_member(*pkey1)) {
			map_iter1 = cl_map_next(map_iter1);
			continue;
		}
		if (ib_pkey_is_full_member(*pkey2)) {
			map_iter2 = cl_map_next(map_iter2);
			continue;
		}

		if (match_pkey(pkey1, pkey2))
			return *pkey1;

		/* advance the lower value if they are not equal */
		pkey1_base = ib_pkey_get_base(cl_map_key(map_iter1));
		pkey2_base = ib_pkey_get_base(cl_map_key(map_iter2));
		if (pkey2_base == pkey1_base) {
			map_iter1 = cl_map_next(map_iter1);
			map_iter2 = cl_map_next(map_iter2);
		} else if (pkey2_base < pkey1_base)
			map_iter2 = cl_map_next(map_iter2);
		else
			map_iter1 = cl_map_next(map_iter1);
	}

	map_iter1 = cl_map_head(&pkey_tbl1->keys);
	map_iter2 = cl_map_head(&pkey_tbl2->keys);

	/* comparing pkey_tbl1 limited with pkey_tbl2 full */
	while ((map_iter1 != cl_map_end(&pkey_tbl1->keys)) &&
	       (map_iter2 != cl_map_end(&pkey_tbl2->keys))) {
		pkey1 = (ib_net16_t *) cl_map_obj(map_iter1);
		pkey2 = (ib_net16_t *) cl_map_obj(map_iter2);

		if (ib_pkey_is_full_member(*pkey1)) {
			map_iter1 = cl_map_next(map_iter1);
			continue;
		}
		if (!ib_pkey_is_full_member(*pkey2)) {
			map_iter2 = cl_map_next(map_iter2);
			continue;
		}

		if (match_pkey(pkey1, pkey2))
			return *pkey1;

		/* advance the lower value if they are not equal */
		pkey1_base = ib_pkey_get_base(cl_map_key(map_iter1));
		pkey2_base = ib_pkey_get_base(cl_map_key(map_iter2));
		if (pkey2_base == pkey1_base) {
			map_iter1 = cl_map_next(map_iter1);
			map_iter2 = cl_map_next(map_iter2);
		} else if (pkey2_base < pkey1_base)
			map_iter2 = cl_map_next(map_iter2);
		else
			map_iter1 = cl_map_next(map_iter1);
	}

	return 0;
}