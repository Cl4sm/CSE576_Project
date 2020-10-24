static int opts_strtoul(uint32_t *val, IN char *p_val_str,
			IN char *p_key, uint32_t max_value)
{
	char *endptr;
	unsigned long int tmp_val;

	errno = 0;
	tmp_val = strtoul(p_val_str, &endptr, 0);
	*val = tmp_val;
	if (*p_val_str == '\0' || *endptr != '\0') {
		log_report("-E- Parsing error in field %s, expected "
			   "numeric input received: %s\n", p_key, p_val_str);
		return -1;
	}
	if (tmp_val > max_value ||
	    ((tmp_val == ULONG_MAX) && errno == ERANGE)) {
		log_report("-E- Parsing error in field %s, value out of range\n", p_key);
		return -1;
	}
	return 0;
}