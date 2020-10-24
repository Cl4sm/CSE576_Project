static inline int unpack_lids(IN char *p_lid_str, OUT uint16_t * p_min_lid,
			      OUT uint16_t * p_max_lid)
{
	unsigned long tmp;
	char *p_next;
	char *p_num;
	char lids_str[24];

	strncpy(lids_str, p_lid_str, 23);
	lids_str[23] = '\0';
	p_num = strtok_r(lids_str, " \t", &p_next);
	if (!p_num)
		return 1;
	tmp = strtoul(p_num, NULL, 0);
	if (tmp >= 0xC000)
		return 1;

	*p_min_lid = (uint16_t) tmp;

	p_num = strtok_r(NULL, " \t", &p_next);
	if (!p_num)
		return 1;
	tmp = strtoul(p_num, NULL, 0);
	if (tmp >= 0xC000)
		return 1;

	*p_max_lid = (uint16_t) tmp;

	return 0;
}