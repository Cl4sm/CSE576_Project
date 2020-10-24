static inline int unpack_neighbor(char *p_str, uint64_t *guid,
				  uint8_t *portnum)
{
	char tmp_str[24];
	char *p_num, *p_next;
	unsigned long tmp_port;

	strncpy(tmp_str, p_str, 23);
	tmp_str[23] = '\0';
	p_num = strtok_r(tmp_str, ":", &p_next);
	if (!p_num)
		return 1;
	if (guid)
		*guid = strtoull(p_num, NULL, 0);

	p_num = strtok_r(NULL, ":", &p_next);
	if (!p_num)
		return 1;
	if (portnum) {
		tmp_port = strtoul(p_num, NULL, 0);
		CL_ASSERT(tmp_port < 0x100);
		*portnum = (uint8_t) tmp_port;
	}

	return 0;
}