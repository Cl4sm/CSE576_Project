boolean_t osm_physp_has_pkey(IN osm_log_t * p_log, IN ib_net16_t pkey,
			     IN const osm_physp_t * p_physp)
{
	ib_net16_t *p_pkey, pkey_base;
	const osm_pkey_tbl_t *pkey_tbl;
	boolean_t res = FALSE;

	OSM_LOG_ENTER(p_log);

	OSM_LOG(p_log, OSM_LOG_DEBUG,
		"Search for PKey: 0x%04x\n", cl_ntoh16(pkey));

	/* if the pkey given is an invalid pkey - return TRUE. */
	if (ib_pkey_is_invalid(pkey)) {
		OSM_LOG(p_log, OSM_LOG_DEBUG,
			"Given invalid PKey - we treat it loosely and allow it\n");
		res = TRUE;
		goto Exit;
	}

	pkey_base = ib_pkey_get_base(pkey);

	pkey_tbl = osm_physp_get_pkey_tbl(p_physp);

	p_pkey = cl_map_get(&pkey_tbl->keys, pkey_base);
	if (p_pkey) {
		res = TRUE;
		OSM_LOG(p_log, OSM_LOG_DEBUG,
			"PKey 0x%04x was found\n", cl_ntoh16(pkey));
	} else
		OSM_LOG(p_log, OSM_LOG_DEBUG,
			"PKey 0x%04x was not found\n", cl_ntoh16(pkey));

Exit:
	OSM_LOG_EXIT(p_log);
	return res;
}