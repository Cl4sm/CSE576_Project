int osm_db_guid2lid_get(IN osm_db_domain_t * p_g2l, IN uint64_t guid,
			OUT uint16_t * p_min_lid, OUT uint16_t * p_max_lid)
{
	char guid_str[20];
	char *p_lid_str;
	uint16_t min_lid, max_lid;

	pack_guid(guid, guid_str);
	p_lid_str = osm_db_lookup(p_g2l, guid_str);
	if (!p_lid_str)
		return 1;
	if (unpack_lids(p_lid_str, &min_lid, &max_lid))
		return 1;

	if (p_min_lid)
		*p_min_lid = min_lid;
	if (p_max_lid)
		*p_max_lid = max_lid;

	return 0;
}