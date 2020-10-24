static u_int32_t bgp_parse_nlri (u_int8_t *buf,
                                 char     *arg)
{
	u_int8_t *ptr = buf;
	char     *arg_ptr = arg;
	char     *new_arg_ptr;
	u_int8_t  bytes;
	
	ptr++;
	(void)bgp_parse_bytes(ptr, arg_ptr, &arg_ptr, 4, 10, '\0');
	*buf = (u_int8_t)strtoul(arg_ptr, &arg_ptr, 10);
	if (*arg_ptr != '\0') {
		arg_ptr++;
	}
	bytes = (u_int8_t)strtoul(arg_ptr, &new_arg_ptr, 10);
	if (arg_ptr != new_arg_ptr) {
		ptr += bytes;
	} else if (*buf > 0) {
		ptr += ((*buf - 1) >> 3) + 1;
	}

	return (ptr - buf);
}
