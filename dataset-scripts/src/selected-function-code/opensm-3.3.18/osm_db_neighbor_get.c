int osm_db_neighbor_get(IN osm_db_domain_t * p_neighbor, IN uint64_t guid1,
			IN uint8_t portnum1, OUT uint64_t * p_guid2,
			OUT uint8_t * p_portnum2)
{
	char neighbor_str[24];
	char *p_other_str;
	uint64_t temp_guid;
	uint8_t temp_portnum;

	pack_neighbor(guid1, portnum1, neighbor_str);
	p_other_str = osm_db_lookup(p_neighbor, neighbor_str);
	if (!p_other_str)
		return 1;
	if (unpack_neighbor(p_other_str, &temp_guid, &temp_portnum))
		return 1;

	if (p_guid2)
		*p_guid2 = temp_guid;
	if (p_portnum2)
		*p_portnum2 = temp_portnum;

	return 0;
}